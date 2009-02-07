 /*****************************************************************************
 * FTPLibFTPTransport functions
 *
 *
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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


static FTPLibFTPTransport_init _FTPLibFTPTransport_init;

FTPLibFTPTransport_init::FTPLibFTPTransport_init() {
	FtpInit();
}

FTPLibFTPTransport_init::~FTPLibFTPTransport_init() {
}


FTPLibFTPTransport::FTPLibFTPTransport(const char *host, StatusReporter *sr) : FTPTransport(host, sr) {

	ftpConnection = 0;
}


FTPLibFTPTransport::~FTPLibFTPTransport() {
	if (ftpConnection)
		FtpQuit(ftpConnection);
}


char FTPLibFTPTransport::assureLoggedIn() {
	char retVal = 0;
	if (ftpConnection == 0) {
		SWLog::getSystemLog()->logDebug("connecting to host %s\n", host.c_str());
		if (FtpConnect(host, &ftpConnection))
			if (FtpLogin(u.c_str(), p.c_str(), ftpConnection)) {
				retVal = 0;
			}
			else {
				SWLog::getSystemLog()->logError("Failed to login to %s\n", host.c_str());
				retVal = -2;
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

	// assert we can login
	retVal = assureLoggedIn();
	if (retVal) return retVal;

	SWBuf sourcePath = sourceURL;
	SWBuf outFile = (!destBuf) ? destPath : "swftplib.tmp";
	sourcePath << (6 + host.length()); // shift << "ftp://hostname";
	SWLog::getSystemLog()->logDebug("getting file %s to %s\n", sourcePath.c_str(), outFile.c_str());
	if (passive)
		FtpOptions(FTPLIB_CONNMODE, FTPLIB_PASSIVE, ftpConnection);
	else
		FtpOptions(FTPLIB_CONNMODE, FTPLIB_PORT, ftpConnection);
	// !!!WDG also want to set callback options
	if (sourcePath.endsWith("/") || sourcePath.endsWith("\\")) {
		SWLog::getSystemLog()->logDebug("getting test directory %s\n", sourcePath.c_str());
		FtpDir(NULL, sourcePath, ftpConnection);
		SWLog::getSystemLog()->logDebug("getting real directory %s\n", sourcePath.c_str());
		retVal = FtpDir(outFile.c_str(), sourcePath, ftpConnection) - 1;
	}
	else {
		SWLog::getSystemLog()->logDebug("getting file %s\n", sourcePath.c_str());
		retVal = FtpGet(outFile.c_str(), sourcePath, FTPLIB_IMAGE, ftpConnection) - 1;
	}

	// Is there a way to FTPGet directly to a buffer?
	// If not, we probably want to add x-platform way to open a tmp file with FileMgr
	// This wreaks and will easily fail if a user's CWD is not writable.
	if (destBuf) {
		FileDesc *fd = FileMgr::getSystemFileMgr()->open("swftplib.tmp", FileMgr::RDONLY);
		long size = fd->seek(0, SEEK_END);
		fd->seek(0, SEEK_SET);
		destBuf->size(size);
		fd->read(destBuf->getRawData(), size);
		FileMgr::getSystemFileMgr()->close(fd);
		FileMgr::removeFile("swftplib.tmp");
	}

	return retVal;
}


SWORD_NAMESPACE_END

