#ifndef INSTALLMGR_H
#define INSTALLMGR_H

#include <stdio.h>
#include <vector>
#include <defs.h>
#include <swbuf.h>
#include <swconfig.h>
#include <map>

SWORD_NAMESPACE_START

class SWMgr;

// move this include to cpp once struct ftpparse isn't exposed anymore
extern "C" {
#include <ftpparse.h>
}


// initialize/cleanup SYSTEMWIDE library with life of this static.
class InstallMgr_init {
public:
	InstallMgr_init();
	~InstallMgr_init();
};


struct FtpFile {
  const char *filename;
  FILE *stream;
};


class InstallSource {
	SWMgr *mgr;
public:
	InstallSource(const char *confEnt, const char *type);
	virtual ~InstallSource();
	SWBuf getConfEnt() {
		return caption +"|" + source + "|" + directory;
	}
	SWBuf type;
	SWBuf source;
	SWBuf directory;
	SWBuf caption;
	SWBuf localShadow;
	void *userData;
	SWMgr *getMgr();
	void flush() { if (mgr) delete mgr; mgr = 0; }
};

typedef std::map < SWBuf, InstallSource * >InstallSourceMap;

class InstallMgr {

protected:
	char *privatePath;
	// probably change to group these ftp functions into some kind of FTPSession
	// class, and open/close functions become c_tor/d_tor.
public:
	SWConfig *installConf;
	InstallSourceMap sources;
	void *FTPOpenSession();
	void FTPCloseSession(void *session);
	char FTPURLGetFile(void *session, const char *dest, const char *sourceurl);

	// probably change to not expose struct ftpparse.  We probably need our
	// own FTPFile class or something that contains things like file name,
	// size, type (dir, file, special).  Then change to vector of this class
	// instead of ftpparse
	std::vector<struct ftpparse> FTPURLGetDir(void *session, const char *dirurl);


public:
	InstallMgr(const char *privatePath = "./");
	virtual ~InstallMgr();
	bool passive;
	bool terminate;
	virtual int removeModule(SWMgr *manager, const char *modName);
	virtual int FTPCopy(InstallSource *is, const char *src, const char *dest, bool dirTransfer = false, const char *suffix = "");
	virtual int installModule(SWMgr *destMgr, const char *fromLocation, const char *modName, InstallSource *is = 0);
	virtual int copyFileToSWORDInstall(SWMgr *manager, const char *sourceDir, const char *fName);
	virtual void refreshRemoteSource(InstallSource *is);
	virtual void statusUpdate(double dltotal, double dlnow);
	virtual void preDownloadStatus(long totalBytes, long completedBytes, const char *message);
};


SWORD_NAMESPACE_END

#endif
