#ifndef INSTALLMGR_H
#define INSTALLMGR_H

#include <stdio.h>
#include <vector>


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

int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
int my_fprogress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

//public stuff


// probably change to group these ftp functions into some kind of FTPSession
// class, and open/close functions become c_tor/d_tor.

void *FTPOpenSession();
void FTPCloseSession(void *session);
char FTPURLGetFile(void *session, const char *dest, const char *sourceurl, bool passive = true, void (*status_callback)(double dltotal, double dlnow)=0);

// probably change to not expose struct ftpparse.  We probably need our
// own FTPFile class or something that contains things like file name,
// size, type (dir, file, special).  Then change to vector of this class
// instead of ftpparse
std::vector<struct ftpparse> FTPURLGetDir(void *session, const char *dirurl, bool passive = true);

#endif
