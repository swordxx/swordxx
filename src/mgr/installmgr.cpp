/*****************************************************************************
 * InstallMgr functions to be made into something usefully exposed by
 * master Glassey
 *
 */
 

#include <installmgr.h>

#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

static InstallMgr_init _InstallMgr_init;

InstallMgr_init::InstallMgr_init() {
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

InstallMgr_init::~InstallMgr_init() {
	curl_global_cleanup();
}


int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) {
	struct FtpFile *out=(struct FtpFile *)stream;
	if (out && !out->stream) {
		/* open file for writing */
		out->stream=fopen(out->filename, "wb");
		if (!out->stream)
			return -1; /* failure, can't open file to write */
	}
	return fwrite(buffer, size, nmemb, out->stream);
}


int my_fprogress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
	if (clientp) {
		(*(void (*)(double, double))clientp)(dltotal, dlnow);
	}
	return 0;
}


char FTPURLGetFile(void *session, const char *dest, const char *sourceurl, void (*status_callback)(double dltotal, double dlnow)) {
	struct FtpFile ftpfile = {dest, NULL};
	char retVal = 0;

	CURL *curl = (CURL *)session;
	CURLcode res;
	
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, sourceurl);
	
		curl_easy_setopt(curl, CURLOPT_USERPWD, "ftp:installmgr@user.com");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, status_callback);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_fprogress);
		/* Set a pointer to our struct to pass to the callback */
		curl_easy_setopt(curl, CURLOPT_FILE, &ftpfile);

		/* Switch on full protocol/debug output */
		curl_easy_setopt(curl, CURLOPT_VERBOSE, TRUE);

		res = curl_easy_perform(curl);

		if(CURLE_OK != res) {
			retVal = -1;
		}
	}

	if (ftpfile.stream)
		fclose(ftpfile.stream); /* close the local file */
	return retVal;
}



vector<struct ftpparse> FTPURLGetDir(void *session, const char *dirurl) {

	vector<struct ftpparse> dirList;
	
	if (!FTPURLGetFile(session, "dirlist", dirurl)) {
		int fd = open("dirlist", O_RDONLY|O_BINARY);
		long size = lseek(fd, 0, SEEK_END);
		lseek(fd, 0, SEEK_SET);
		char *buf = new char [ size + 1 ];
		read(fd, buf, size);
		close(fd);
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
			int status = ftpparse(&item, start, end - start);
			if (status)
				dirList.push_back(item);
			start = end;
		}
	}
	return dirList;
}


void *FTPOpenSession() {
	CURL *curl;

	curl = curl_easy_init();
	return curl;
}


void FTPCloseSession(void *session) {
	CURL *curl = (CURL *)session;
	curl_easy_cleanup(curl);
}
    