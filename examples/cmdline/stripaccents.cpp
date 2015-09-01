/******************************************************************************
 *
 *  search.cpp -	This simple example shows how to perform a search on a
 *			SWORD module. It amounts to a simple commandline
 *			search tool with a usage like:
 *
 *				search KJV "swift hear slow speak"
 *
 * $Id: search.cpp 3269 2014-10-09 14:55:14Z scribe $
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <stdio.h>
#include <cstdlib>
#include <swmgr.h>
#include <markupfiltmgr.h>
#include <iostream>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int usage()
{
		fprintf(stderr, "\nusage: stripaccents <n=1-15> <text-to-be-stripped> \n");
		fprintf(stderr, "\n n=1 \t strip Greek accents");
		fprintf(stderr, "\n n=2 \t strip Arabic vowel points");
		fprintf(stderr, "\n n=4 \t strip Hebrew vowel points");
		fprintf(stderr, "\n n=8 \t strip Hebrew Cantillation marks\n");
		fprintf(stderr, "\n several of the above filters can be engaged by adding the values\n");
		exit(0);
}        

int main(int argc, char **argv)
{
//	SWMgr manager(0, 0, true, new MarkupFilterMgr(FMT_RTF, ENC_RTF));
	SWMgr manager;

	if ((argc < 3) || (argc > 3)) {
	        usage();
		exit(-1);
	}

        int stripFilters = atoi(argv[1]);
        
        if ((stripFilters < 1) || (stripFilters > 15)) {
                usage();
		exit(-1);
        }        
	SWBuf stripTerm = argv[2];
	
	manager.setGlobalOption("Greek Accents", "Off");
	manager.setGlobalOption("Arabic Vowel Points", "Off");
	manager.setGlobalOption("Hebrew Vowel Points", "Off");
	manager.setGlobalOption("Hebrew Cantillation", "Off");
	
	if (stripFilters & (1 << 0)) manager.filterText("Hebrew Vowel Points", stripTerm);
	if (stripFilters & (1 << 1)) manager.filterText("Hebrew Cantillation", stripTerm);
	if (stripFilters & (1 << 2)) manager.filterText("Arabic Vowel Points", stripTerm);
	if (stripFilters & (1 << 3)) manager.filterText("Greek Accents", stripTerm);
	
	fprintf(stdout,"%s\n",stripTerm.c_str());
	return 0;

}
