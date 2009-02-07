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
#include <papyriplain.h>
//#include <swmgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using namespace std;


int main(int argc, char **argv) {
//	SWMgr mgr;
//	SWModule *module = mgr.getModule("KJV");
	PapyriPlain filter;
	SWBuf buf;
	buf = "This is t<e>xt which has papy-\nri markings in it.\n  L[et's be] sure it gets--\n cleaned up well for s(earching)";
	std::cout << "Original:\n\n" << buf << "\n\n-------\n\n";
	filter.processText(buf);
//	filter.processText(buf, module->getKey(), module);
	std::cout << buf << "\n\n+++++++\n";

	return 0;
}
