 /*****************************************************************************
 * FTPTransport functions
 *
 */
 

#include <ftptrans.h>
#include <filemgr.h>

#include <fcntl.h>
#include <dirent.h>
#include <swlog.h>


using std::vector;


SWORD_NAMESPACE_START


namespace {

void removeTrailingSlash(SWBuf &buf) {
	int len = buf.size();
	if ((buf[len-1] == '/')
	 || (buf[len-1] == '\\'))
		buf.size(len-1);
}

};


void StatusReporter::preStatus(long totalBytes, long completedBytes, const char *message) {
}

void StatusReporter::statusUpdate(double dtTotal, double dlNow) {
}


FTPTransport::FTPTransport(const char *host, StatusReporter *statusReporter) {
	this->statusReporter = statusReporter;
	this->host = host;
	term = false;
}


FTPTransport::~FTPTransport() {
}


// override this method in your real transport class
char FTPTransport::getURL(const char *destPath, const char *sourceURL) {
	char retVal = 0;
	return retVal;
}


vector<struct ftpparse> FTPTransport::getDirList(const char *dirURL) {

	vector<struct ftpparse> dirList;
	
	if (!getURL("dirlist", dirURL)) {
		FileDesc *fd = FileMgr::getSystemFileMgr()->open("dirlist", FileMgr::RDONLY);
		long size = fd->seek(0, SEEK_END);
		fd->seek(0, SEEK_SET);
		char *buf = new char [ size + 1 ];
		fd->read(buf, size);
		FileMgr::getSystemFileMgr()->close(fd);
		char *start = buf;
		char *end = start;
		while (start < (buf+size)) {
			struct ftpparse item;
			bool looking = true;
			for (end = start; *end; end++) {
				if (looking) {
					if ((*end == 10) || (*end == 13)) {
						*end = 0;
						looking = false;
					}
				}
				else if ((*end != 10) && (*end != 13))
					break;
			}
			SWLog::getSystemLog()->logWarning("FTPURLGetDir: parsing item %s(%d)\n", start, end-start);
			int status = ftpparse(&item, start, end - start);
			SWLog::getSystemLog()->logWarning("FTPURLGetDir: got item %s\n", item.name);
			if (status)
				dirList.push_back(item);
			start = end;
		}
	}
	else
	{
		SWLog::getSystemLog()->logWarning("FTPURLGetDir: failed to get dir %s\n", dirURL);
	}
	return dirList;
}


int FTPTransport::copyDirectory(const char *urlPrefix, const char *dir, const char *dest, const char *suffix) {
	unsigned int i;
	int retVal = 0;
	
	SWBuf url = SWBuf(urlPrefix) + SWBuf(dir);
	removeTrailingSlash(url);
	url += '/';
	
	SWLog::getSystemLog()->logWarning("FTPCopy: getting dir %s\n", url.c_str());
	vector<struct ftpparse> dirList = getDirList(url.c_str());

	if (!dirList.size()) {
		SWLog::getSystemLog()->logWarning("FTPCopy: failed to read dir %s\n", url.c_str());
		return -1;
	}
				
	long totalBytes = 0;
	for (i = 0; i < dirList.size(); i++)
		totalBytes += dirList[i].size;
	long completedBytes = 0;
	for (i = 0; i < dirList.size(); i++) {
		struct ftpparse &dirEntry = dirList[i];
		SWBuf buffer = (SWBuf)dest;
		removeTrailingSlash(buffer);
		buffer += (SWBuf)"/" + (dirEntry.name);
		if (!strcmp(&buffer.c_str()[buffer.length()-strlen(suffix)], suffix)) {
			SWBuf buffer2 = "Downloading (";
			buffer2.appendFormatted("%d", i+1);
			buffer2 += " of ";
			buffer2.appendFormatted("%d", dirList.size());
			buffer2 += "): ";
			buffer2 += (dirEntry.name);
			if (statusReporter)
				statusReporter->preStatus(totalBytes, completedBytes, buffer2.c_str());
			FileMgr::createParent(buffer.c_str());	// make sure parent directory exists
			SWTRY {
				SWBuf url = (SWBuf)urlPrefix + (SWBuf)dir;
				removeTrailingSlash(url);
				url += (SWBuf)"/" + dirEntry.name; //dont forget the final slash
				if (dirEntry.flagtrycwd != 1) {
					if (getURL(buffer.c_str(), url.c_str())) {
						SWLog::getSystemLog()->logWarning("FTPCopy: failed to get file %s\n", url.c_str());
						return -2;
					}
					completedBytes += dirEntry.size;
				}
				else {
					SWBuf subdir = (SWBuf)dir;
					removeTrailingSlash(subdir);
					subdir += (SWBuf)"/" + dirEntry.name;
					if (copyDirectory(urlPrefix, subdir, buffer.c_str(), suffix)) {
						SWLog::getSystemLog()->logWarning("FTPCopy: failed to get file %s\n", subdir.c_str());
						return -2;
					}
				}
			}
			SWCATCH (...) {}
			if (term) {
				retVal = -3;
				break;
			}
		}
	}
	return retVal;
}


SWORD_NAMESPACE_END

