 /*****************************************************************************
 * FTPLibFTPTransport functions
 *
 */
 
#include <stdio.h>
#include <ftplibftpt.h>

#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#include <ftplib.h>


SWORD_NAMESPACE_START


static FTPLibFTPTransport_init _FTPLibFTPTransport_init;

FTPLibFTPTransport_init::FTPLibFTPTransport_init() {
	FtpInit();
}

FTPLibFTPTransport_init::~FTPLibFTPTransport_init() {
}


FTPLibFTPTransport::FTPLibFTPTransport(const char *host, StatusReporter *sr) : FTPTransport(host, sr) {
	void *retVal = 0;

	fprintf(stderr, "connecting to host %s\n", host);
	if (FtpConnect(host, (netbuf **)&nControl))
		retVal = nControl;
	else
		fprintf(stderr, "Failed to connect to %s\n", host);
	if (!FtpLogin("anonymous", "installmgr@user.com", (netbuf *)nControl))
		fprintf(stderr, "Failed to login to %s\n", host);
}


FTPLibFTPTransport::~FTPLibFTPTransport() {
	FtpQuit((netbuf *) nControl);
}


char FTPLibFTPTransport::getURL(const char *destPath, const char *sourceURL) {
	char retVal = 0;
	fprintf(stderr, "getting file %s to %s\n", sourceURL, destPath);
	if (passive)
		FtpOptions(FTPLIB_CONNMODE, FTPLIB_PASSIVE, (netbuf *)nControl);
	else
		FtpOptions(FTPLIB_CONNMODE, FTPLIB_PORT, (netbuf *)nControl);
	// !!!WDG also want to set callback options
	if (!strcmp(destPath, "dirlist")) {
		fprintf(stderr, "getting test directory %s\n", sourceURL);
		FtpDir(NULL, sourceURL, (netbuf *)nControl);
		fprintf(stderr, "getting real directory %s\n", sourceURL);
		retVal = FtpDir(destPath, sourceURL, (netbuf *)nControl) - 1;
	}
	else {
		fprintf(stderr, "getting file %s\n", sourceURL);
		retVal = FtpGet(destPath, sourceURL, FTPLIB_IMAGE, (netbuf *)nControl) - 1;
	}

	return retVal;
}


SWORD_NAMESPACE_END

