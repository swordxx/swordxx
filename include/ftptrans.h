#ifndef FTPTRANS_H
#define FTPTRANS_H

#include <vector>
#include <defs.h>
#include <swbuf.h>

//SWORD_NAMESPACE_START


// move this include to cpp once struct ftpparse isn't exposed anymore
extern "C" {
#include <ftpparse.h>
}

SWORD_NAMESPACE_START

/** Class for reporting status
*/
class SWDLLEXPORT StatusReporter {
public:
	/** Messages before stages of a batch download */
	virtual void preStatus(long totalBytes, long completedBytes, const char *message);
	
	/** frequently called throughout a download, to report status */
	virtual void statusUpdate(double dtTotal, double dlNow);
};


/** TODO: document
* A base class to be used for reimplementation of network services.
*/
class SWDLLEXPORT FTPTransport {

protected:
	StatusReporter *statusReporter;
	bool passive;
	bool term;
	SWBuf host;
	
public:
	FTPTransport(const char *host, StatusReporter *statusReporter = 0);
	virtual ~FTPTransport();
	virtual char getURL(const char *destPath, const char *sourceURL);

	int copyDirectory(const char *urlPrefix, const char *dir, const char *dest, const char *suffix);

	// probably change to not expose struct ftpparse.  We probably need our
	// own FTPFile class or something that contains things like file name,
	// size, type (dir, file, special).  Then change to vector of this class
	// instead of ftpparse
	virtual std::vector<struct ftpparse> getDirList(const char *dirURL);
	void setPassive(bool passive) { this->passive = passive; }
	void terminate() { term = true; }
};


SWORD_NAMESPACE_END

#endif
