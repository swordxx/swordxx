#include <iostream>
#include <rawstr.h>
#include <swmgr.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <lex path>\n\n", *argv);
		exit(-1);
	}
	
	RawStr mod(argv[1]);
     char buf[127];

	sprintf(buf, "%s.idx", argv[1]);
	FileDesc *idxfd = FileMgr::systemFileMgr.open(buf, O_RDONLY|O_BINARY, true);
	long maxoff = lseek(idxfd->getFd(), 0, SEEK_END) - 6;
	FileMgr::systemFileMgr.close(idxfd);

	std::string last = "";
	bool first = true;
	char *trybuf = 0;
	for (long index = 0; index < maxoff; index+=6) {
		mod.getIDXBuf(index, &trybuf);
		if (!first) {
			if (strcmp(trybuf, last.c_str()) < 0) {
				printf("entry %ld(offset: %ld) (%s) is less than previous entry (%s)\n\n", index/6, index, trybuf, last.c_str());
				exit(-3);
			}
		}
		else first = false;
		last = trybuf;
	}
	if (trybuf)
		delete [] trybuf;

	return 0;
}
