/*****************************************************************************
 *
 *  ftplibftpt.cpp -	FTPLibFTPTransport
 *
 * $Id$
 *
 * Copyright 2004-2013 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */
 
#include <stdio.h>
#include <fcntl.h>

#include <ftplib.h>

#include <ftplibftpt.h>
#include <swlog.h>
#include <filemgr.h>


SWORD_NAMESPACE_START

namespace {

	struct MyProgressData {
		StatusReporter *sr;
		long totalSize;
		bool *term;
	};

	static int my_swbufwriter(netbuf *nControl, void *buffer, size_t size, void *swbuf) {
		SWBuf &output = *(SWBuf *)swbuf;
		int s = output.size();
		output.size(s+size);
		memcpy(output.getRawData()+s, buffer, size);
		return size;
	}

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

	static int my_fprogress(netbuf *nControl, int xfered, void *arg) {
		if (arg) {
			MyProgressData *pd = (MyProgressData *)arg;
	SWLog::getSystemLog()->logDebug("FTPLibFTPTransport report progress: totalSize: %ld; xfered: %d\n", pd->totalSize, xfered);
			if (pd->sr) {
				pd->sr->statusUpdate(pd->totalSize, xfered);
			}
			if (*(pd->term)) return 0;
		}
		return 1;
	}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif


	// initialize/cleanup SYSTEMWIDE library with life of this static.
	static class FTPLibFTPTransport_init {
	public:
		FTPLibFTPTransport_init() {
			FtpInit();
		}

		~FTPLibFTPTransport_init() {
		}

	} _ftpLibFTPTransport_init;

}


FTPLibFTPTransport::FTPLibFTPTransport(const char *host, StatusReporter *sr) : RemoteTransport(host, sr) {

	ftpConnection = 0;
}


FTPLibFTPTransport::~FTPLibFTPTransport() {
	if (ftpConnection)
		FtpQuit(ftpConnection);
}


char FTPLibFTPTransport::assureLoggedIn() {
	char retVal = 0;
	if (ftpConnection == 0) {
		SWLog::getSystemLog()->logDebug("connecting to host: %s...\n", host.c_str());
		if (FtpConnect(host, &ftpConnection)) {
			FtpOptions(FTPLIB_CONNMODE, (passive) ? FTPLIB_PASSIVE : FTPLIB_PORT, ftpConnection);

			SWLog::getSystemLog()->logDebug("connected. logging in...\n");
			if (FtpLogin(u.c_str(), p.c_str(), ftpConnection)) {
				SWLog::getSystemLog()->logDebug("logged in.\n");
				retVal = 0;
			}
			else {
				SWLog::getSystemLog()->logError("Failed to login to %s\n", host.c_str());
				retVal = -2;
			}
		}
		else {
			SWLog::getSystemLog()->logError("Failed to connect to %s\n", host.c_str());
			retVal = -1;
		}
	}
	return retVal;
}


char FTPLibFTPTransport::getURL(const char *destPath, const char *sourceURL, SWBuf *destBuf) {

	char retVal = 0;

	SWLog::getSystemLog()->logDebug("FTPLibFTPTransport::getURL(%s, %s, ...);\n", (destPath)?destPath:"(null)", sourceURL);
	// assert we can login
	retVal = assureLoggedIn();
	if (retVal) return retVal;
	SWLog::getSystemLog()->logDebug("FTPLibFTPTransport - logged in.\n");

	SWBuf sourcePath = sourceURL;

	SWBuf outFile;
	if (!destBuf) {
		outFile = destPath;
	}

	sourcePath << (6 + host.length()); // shift << "ftp://hostname";
	SWLog::getSystemLog()->logDebug("getting file %s to %s\n", sourcePath.c_str(), destBuf ? "*internal buffer*" : outFile.c_str());
	struct MyProgressData pd;
	pd.sr = statusReporter;
	pd.term = &term;
	pd.totalSize = 0;

	if (destBuf) {
		FtpOptions(FTPLIB_CALLBACK_WRITER, (long)&my_swbufwriter, ftpConnection);
		FtpOptions(FTPLIB_CALLBACK_WRITERARG, (long)destBuf, ftpConnection);
	}
	else {
		FtpOptions(FTPLIB_CALLBACK_WRITER, 0L, ftpConnection);
	}

	FtpOptions(FTPLIB_CALLBACK, (long)&my_fprogress, ftpConnection);
	FtpOptions(FTPLIB_CALLBACKARG, (long)&pd, ftpConnection);
	FtpOptions(FTPLIB_CALLBACKBYTES, (long)2048, ftpConnection);

	if (sourcePath.endsWith("/") || sourcePath.endsWith("\\")) {
//		SWLog::getSystemLog()->logDebug("getting test directory %s\n", sourcePath.c_str());
//		FtpDir(NULL, sourcePath, ftpConnection);
		SWLog::getSystemLog()->logDebug("getting real directory %s\n", sourcePath.c_str());
		retVal = FtpDir(destBuf ? 0 : outFile.c_str(), sourcePath, ftpConnection) - 1;
		SWLog::getSystemLog()->logDebug("got real directory %s to %s\n", sourcePath.c_str(), destBuf ? "*internal buffer*" : outFile.c_str());
	}
	else {
		SWLog::getSystemLog()->logDebug("getting file %s\n", sourcePath.c_str());
		int size;
		FtpSize(sourcePath, &size, FTPLIB_IMAGE, ftpConnection);
		pd.totalSize = size;
		retVal = FtpGet(destBuf ? 0 : outFile.c_str(), sourcePath, FTPLIB_IMAGE, ftpConnection) - 1;
	}

	SWLog::getSystemLog()->logDebug("FTPLibFTPTransport - returning: %d\n", retVal);
	return retVal;
}


SWORD_NAMESPACE_END

