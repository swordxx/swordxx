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
#include <rawtext.h>
#include <rawtext4.h>
#include <versekey.h>
#include <swbuf.h>
#include <filemgr.h>
#include <stdio.h>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

using namespace std;

void writeEntry(const SWBuf &key, const SWBuf &entry, SWModule *module);

void usage(const char *progName, const char *error = 0) {
	if (error) fprintf(stderr, "\n%s: %s\n", progName, error);
	fprintf(stderr, "\n=== imp2vs (Revision $Rev: 2234 $) SWORD Bible/Commentary importer.\n");
	fprintf(stderr, "\nusage: %s <imp_file> [options]\n", progName);
	fprintf(stderr, "  -a\t\t\t augment module if exists (default is to create new)\n");
	fprintf(stderr, "  -o <output_path>\t where to write data files.\n");
	fprintf(stderr, "  -4\t\t\t use 4 byte size entries (default is 2).\n");
	fprintf(stderr, "  -v <v11n>\t\t specify a versification scheme to use (default is KJV)\n");
	fprintf(stderr, "\t\t\t\t Note: The following are valid values for v11n:\n");
	VerseMgr *vmgr = VerseMgr::getSystemVerseMgr();
	StringList av11n = vmgr->getVersificationSystems();
	for (StringList::iterator loop = av11n.begin(); loop != av11n.end(); loop++) {
		fprintf(stderr, "\t\t\t\t\t%s\n", (*loop).c_str());
        }
	fprintf(stderr, "\n");
	fprintf(stderr, "'imp' format is a simple standard for importing data into SWORD modules.\n"
		"Required is a plain text file containing $$$key lines followed by content.\n\n"
		"$$$Gen.1.1\n"
		"In the beginning God created\n"
		"the heavens and the earth\n"
		"$$$Gen.1.2\n"
		"and the earth...\n\n"
		"Key lines can contain ranges, for example, a commentary entry which discusses\n"
		"John 1:1-4 might have a key, $$$Jn.1.1-4.  Special keys for intro entries use\n"
		"standard SWORD notation, e.g. $$$Rom.4.0 for intro of Romans chapter 4,\n"
		"$$$Rev.0.0 for intro of the Book of Revelation of John.  $$$[ Module Heading ]\n"
		"for entire module intro.  $$$[ Testament 2 Heading ] for NT intro.\n\n");
	exit(-1);
}


int main(int argc, char **argv) {


	// handle options
	if (argc < 2) usage(*argv);

	const char *progName   = argv[0];
	const char *inFileName = argv[1];
	SWBuf v11n             = "KJV";
	SWBuf outPath          = "./";
	bool fourByteSize      = false;
	bool append            = false;

	for (int i = 2; i < argc; i++) {
		if (!strcmp(argv[i], "-a")) {
			append = true;
		}
		if (!strcmp(argv[i], "-4")) {
			fourByteSize = true;
		}
		else if (!strcmp(argv[i], "-o")) {
			if (i+1 < argc) outPath = argv[++i];
			else usage(progName, "-o requires <output_path>");
		}
		else if (!strcmp(argv[i], "-v")) {
			if (i+1 < argc) v11n = argv[++i];
			else usage(progName, "-v requires <v11n>");
		}
		else usage(progName, (((SWBuf)"Unknown argument: ")+ argv[i]).c_str());
	}
	// -----------------------------------------------------
	const VerseMgr::System *v = VerseMgr::getSystemVerseMgr()->getVersificationSystem(v11n);
	if (!v) std::cout << "Warning: Versification " << v11n << " not found. Using KJV versification...\n";


	// setup module
	if (!append) {
		if (!fourByteSize)
			RawText::createModule(outPath, v11n);
		else	RawText4::createModule(outPath, v11n);
	}

	SWModule *module = (!fourByteSize)
			? (SWModule *)new RawText(outPath, 0, 0, 0, ENC_UNKNOWN, DIRECTION_LTR, FMT_UNKNOWN, 0, v11n)
			: (SWModule *)new RawText4(outPath, 0, 0, 0, ENC_UNKNOWN, DIRECTION_LTR, FMT_UNKNOWN, 0, v11n);
	// -----------------------------------------------------
			

	// setup module key to allow full range of possible values, and then some
	VerseKey *vkey = (VerseKey *)module->CreateKey();
	vkey->Headings(1);
	vkey->AutoNormalize(0);
	vkey->Persist(1);
	module->setKey(*vkey);
	// -----------------------------------------------------


	// process input file
	FileDesc *fd = FileMgr::getSystemFileMgr()->open(inFileName, FileMgr::RDONLY);

	SWBuf lineBuffer;
	SWBuf currentKey;
	SWBuf currentEntry;

	while (FileMgr::getLine(fd, lineBuffer)) {
		if (lineBuffer.startsWith("$$$")) {
			writeEntry(currentKey, currentEntry, module);
			currentKey = lineBuffer;
			currentKey << 3;
			currentKey.trim();
			currentEntry = "";
		}
		else {
			currentEntry += lineBuffer;
		}
	}
	writeEntry(currentKey, currentEntry, module);

	FileMgr::getSystemFileMgr()->close(fd);

	delete vkey;

	return 0;
}



int page = 0;


void writeEntry(const SWBuf &key, const SWBuf &entry, SWModule *module)
{
	if (key.size() && entry.size()) {
		std::cout << "from file: " << key << std::endl;
		VerseKey *vkey = (VerseKey *)module->getKey();
		VerseKey *linkMaster = (VerseKey *)module->CreateKey();

		ListKey listKey = vkey->ParseVerseList(key.c_str(), "Gen1:1", true);

		bool first = true;
		for (listKey = TOP; !listKey.Error(); listKey++) {
			*vkey = listKey;
			if (first) {
				*linkMaster = *vkey;
				SWBuf text = module->getRawEntry();
				text += entry;


				//------------------------------------------------------------
				//  Tregelles Page marking special stuff
				//------------------------------------------------------------
/*
				const char *pageMarker = "<seg type=\"page\" subtype=\"";
				int newPage = page;
				SWBuf pageData = strstr(text.c_str(), pageMarker);
				if (pageData.length()) {
					pageData << strlen(pageMarker);
					const char *pn = pageData.stripPrefix('"');
					if (pn) newPage = atoi(pn);
				}
				// add page stuff for treg
				if (text.startsWith(pageMarker)) {
					// don't add anything cuz we already start with one
				}
				else {
					SWBuf pm = pageMarker;
					pm.appendFormatted("%d\" />", page);
					text = pm + text;
				}

				page = newPage;	// when our line set a new page number

*/
				//------------------------------------------------------------




				std::cout << "adding entry: " << *vkey << " length " << entry.size() << "/" << (unsigned short)text.size() << std::endl;
				module->setEntry(text);
				first = false;
			}
			else {
				std::cout << "linking entry: " << *vkey << " to " << *linkMaster << std::endl;
				module->linkEntry(linkMaster);
			}
		}

		delete linkMaster;
	}
}
