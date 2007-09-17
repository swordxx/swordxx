 /*****************************************************************************
 * FTPLibFTPTransport functions
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
	void *retVal = 0;

	SWLog::getSystemLog()->logDebug("connecting to host %s\n", host);
	if (FtpConnect(host, (netbuf **)&nControl))
		retVal = nControl;
	else
		SWLog::getSystemLog()->logDebug("Failed to connect to %s\n", host);
	if (!FtpLogin("anonymous", "installmgr@user.com", (netbuf *)nControl))
		SWLog::getSystemLog()->logDebug("Failed to login to %s\n", host);
}


FTPLibFTPTransport::~FTPLibFTPTransport() {
	FtpQuit((netbuf *) nControl);
}


char FTPLibFTPTransport::getURL(const char *destPath, const char *sourceURL, SWBuf *destBuf) {
	char retVal = 0;
	SWBuf sourcePath = sourceURL;
	SWBuf outFile = (!destBuf) ? destPath : "swftplib.tmp";
	sourcePath << (6 + host.length()); // shift << "ftp://hostname";
	SWLog::getSystemLog()->logDebug("getting file %s to %s\n", sourcePath.c_str(), outFile.c_str());
	if (passive)
		FtpOptions(FTPLIB_CONNMODE, FTPLIB_PASSIVE, (netbuf *)nControl);
	else
		FtpOptions(FTPLIB_CONNMODE, FTPLIB_PORT, (netbuf *)nControl);
	// !!!WDG also want to set callback options
	if (sourcePath.endsWith("/") || sourcePath.endsWith("\\")) {
		SWLog::getSystemLog()->logDebug("getting test directory %s\n", sourcePath.c_str());
		FtpDir(NULL, sourcePath, (netbuf *)nControl);
		SWLog::getSystemLog()->logDebug("getting real directory %s\n", sourcePath.c_str());
		retVal = FtpDir(outFile.c_str(), sourcePath, (netbuf *)nControl) - 1;
	}
	else {
		SWLog::getSystemLog()->logDebug("getting file %s\n", sourcePath.c_str());
		retVal = FtpGet(outFile.c_str(), sourcePath, FTPLIB_IMAGE, (netbuf *)nControl) - 1;
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

