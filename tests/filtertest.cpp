#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif
#ifndef O_BINARY
#define O_BINARY 0
#endif
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <thmlhtmlhref.h>
#include <unicodertf.h>
#include <thmlosis.h>
#include <gbfosis.h>
#include <thmlosis.h>
#include <versekey.h>
#include <swmgr.h>
#include <markupfiltmgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using namespace std;

#define MAXBUF 30000
char readline(int fd, char **buf) {
	char ch;
	if (*buf)
		delete [] *buf;
	*buf = 0;
	int len;


	long index = lseek(fd, 0, SEEK_CUR);
	// clean up any preceding white space
	while ((len = read(fd, &ch, 1)) == 1) {
		if ((ch != 13) && (ch != ' ') && (ch != '\t'))
			break;
		else index++;
	}


	while (ch != 10) {
        if ((len = read(fd, &ch, 1)) != 1)
			break;
	}
	
	int size = (lseek(fd, 0, SEEK_CUR) - index) - 1;

	*buf = new char [ size + 1 ];

	if (size > 0) {
		lseek(fd, index, SEEK_SET);
		read(fd, *buf, size);
		read(fd, &ch, 1);   //pop terminating char
		(*buf)[size] = 0;

		// clean up any trailing junk on buf
		for (char *it = *buf+(strlen(*buf)-1); it > *buf; it--) {
			if ((*it != 10) && (*it != 13) && (*it != ' ') && (*it != '\t'))
				break;
			else *it = 0;
		}
	}
	else **buf = 0;
	return !len;
}
int main(int argc, char **argv) {
/*
	SWMgr mgr(0, 0, true, new MarkupFilterMgr(FMT_HTMLHREF, ENC_RTF));
	mgr.setGlobalOption("Strong's Numbers", "on");
	mgr.setGlobalOption("Morphological Tags", "on");
	SWModule *module = mgr.Modules["KJV2003"];
	if (!module)
		module = mgr.Modules.begin()->second;
*/

    //ThMLOSIS filter;
	int fd = open(argv[1], O_RDONLY|O_BINARY);
	if (fd < 0) {
		fprintf(stderr, "error: %s: couldn't open input file: %s \n", argv[0], argv[2]);
		exit(-2);
	}
	UnicodeRTF filter;
	char *buffer = 0;
	while (!readline(fd, &buffer)) {
		SWBuf buf = buffer;
		filter.processText(buf);
		cout << buf << "\n";
	}
//    module->Key() = ((argc > 1) ? argv[1] : "john 1:1");
    /*
    char *buf = new char [ MAXBUF ];
    memset(buf, 0, MAXBUF);
//    strcpy(buf, "This is a verse reference: <scripRef>jas1:22,23-25;3;5:1;rom1-9</scripRef> with an <img src=\"/images/yoyo.jpg\">");
    module->getRawEntry();
    memcpy(buf, module->getRawEntry(), module->getEntrySize());
    std::cout << "Original:\n\n" << buf << "\n\n-------\n\n";
    filter.ProcessText(buf, MAXBUF - 3, *module, module);

    std::cout << buf << "\n\n+++++++\n";
    delete [] buf;
    cout << module->Name() << " : " << module->KeyText() << "\n";
    cout << module->RenderText() << "\n";
    */
    return 0;
}
