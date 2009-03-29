/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <iostream>
#include <swbuf.h>
#include <filemgr.h>
#include <papyriplain.h>
#include <utf8utf16.h>
//#include <swmgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using namespace std;


int main(int argc, char **argv) {
	UTF8UTF16 filter;
//	PapyriPlain filter;
//
	FileDesc *fd = (argc > 1) ? FileMgr::getSystemFileMgr()->open(argv[1], FileMgr::RDONLY) : 0;

	SWBuf lineBuffer = "This is t<e>xt which has papy-\nri markings in it.\n  L[et's be] sure it gets--\n cleaned up well for s(earching)";

	std::cout << "Original:\n\n";

	while (!fd || FileMgr::getLine(fd, lineBuffer)) {
		cout << lineBuffer << "\n";
		if (!fd) break;
	}

 	cout << "\n\n-------\n\n";

	if (fd) {
		FileMgr::getSystemFileMgr()->close(fd);
		fd = FileMgr::getSystemFileMgr()->open(argv[1], FileMgr::RDONLY);
	}

	while (!fd || FileMgr::getLine(fd, lineBuffer)) {
		filter.processText(lineBuffer);
		for (unsigned int i = 0; i < lineBuffer.size(); i++) {
			printf("%c", lineBuffer[i]);
		}
		cout << "\n";
		if (!fd) break;
	}

	std::cout << "\n\n+++++++\n";

	if (fd) {
		FileMgr::getSystemFileMgr()->close(fd);
	}

	return 0;
}

