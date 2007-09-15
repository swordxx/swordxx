 /*****************************************************************************
 * CURLFTPTransport functions
 *
 */
 

#include <curlftpt.h>

#include <fcntl.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

SWORD_NAMESPACE_START


struct FtpFile {
  const char *filename;
  FILE *stream;
};


int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
int my_fprogress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

static CURLFTPTransport_init _CURLFTPTransport_init;

CURLFTPTransport_init::CURLFTPTransport_init() {
	//curl_global_init(CURL_GLOBAL_DEFAULT);  // curl_easy_init automatically calls it if needed
}

CURLFTPTransport_init::~CURLFTPTransport_init() {
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
		((StatusReporter *)clientp)->statusUpdate(dltotal, dlnow);
	}
	return 0;
}


CURLFTPTransport::CURLFTPTransport(const char *host, StatusReporter *sr) : FTPTransport(host, sr) {
	session = (CURL *)curl_easy_init();
}


CURLFTPTransport::~CURLFTPTransport() {
	curl_easy_cleanup(session);
}


char CURLFTPTransport::getURL(const char *destPath, const char *sourceURL) {
	signed char retVal = 0;
	struct FtpFile ftpfile = {destPath, NULL};

	CURLcode res;
	
	if (session) {
		curl_easy_setopt(session, CURLOPT_URL, sourceURL);
	
		curl_easy_setopt(session, CURLOPT_USERPWD, "ftp:installmgr@user.com");
		curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, my_fwrite);
		if (!passive)
			curl_easy_setopt(session, CURLOPT_FTPPORT, "-");
		curl_easy_setopt(session, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(session, CURLOPT_PROGRESSDATA, statusReporter);
		curl_easy_setopt(session, CURLOPT_PROGRESSFUNCTION, my_fprogress);
		/* Set a pointer to our struct to pass to the callback */
		curl_easy_setopt(session, CURLOPT_FILE, &ftpfile);

		/* Switch on full protocol/debug output */
		curl_easy_setopt(session, CURLOPT_VERBOSE, true);
		
		/* FTP connection settings */

#if (LIBCURL_VERSION_MAJOR > 7) || \
   ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 10)) || \
   ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR == 10) && (LIBCURL_VERSION_PATCH >= 5))
#      define EPRT_AVAILABLE 1
#endif

#ifdef EPRT_AVAILABLE
		curl_easy_setopt(session, CURLOPT_FTP_USE_EPRT, 0);
fprintf(stderr, "***** using CURLOPT_FTP_USE_EPRT\n");
#endif

		
fprintf(stderr, "***** About to perform curl easy action. \n");
fprintf(stderr, "***** destPath: %s \n", destPath);
fprintf(stderr, "***** sourceURL: %s \n", sourceURL);
		res = curl_easy_perform(session);
fprintf(stderr, "***** Finished performing curl easy action. \n");

		if(CURLE_OK != res) {
			retVal = -1;
		}
	}

	if (ftpfile.stream)
		fclose(ftpfile.stream); /* close the local file */

	return retVal;
}


SWORD_NAMESPACE_END

