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

using namespace std;

#include <iostream>
#include <rawtext.h>
 #include <rawtext4.h>
#include <versekey.h>
#include <swbuf.h>
#include <filemgr.h>


#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

void writeEntry(const SWBuf &key, const SWBuf &entry, SWModule *module);

void usage(const char *progName, const char *error = 0) {
	if (error) fprintf(stderr, "\n%s: %s\n", progName, error);
	fprintf(stderr, "\n=== imp2vs (Revision $Rev: 2234 $) SWORD Bible/Commentary importer.\n");
	fprintf(stderr, "\nusage: %s <imp_file> [options]\n"
		"\t -a augment module if exists (default is to create new)\n"
		"\t -o <output_path> where to write data files.\n"
		"\t -4 use 4 byte size entries (default is 2).\n\n"
		"'imp' format is a simple standard for importing data into SWORD modules.\n"
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
		"for entire module intro.  $$$[ Testament 2 Heading ] for NT intro.\n\n"
		, progName);
	exit(-1);
}


int main(int argc, char **argv) {


	// handle options
	if (argc < 2) usage(*argv);

	const char *progName   = argv[0];
	const char *inFileName = argv[1];
	SWBuf outPath          = "./";
	bool fourByteSize      = false;
	bool append            = false;

	for (int i = 2; i < argc; i++) {
		if (!strcmp(argv[i], "-a")) {
			append = true;
		}
		else if (!strcmp(argv[i], "-o")) {
			if (i+1 < argc) outPath = argv[++i];
			else usage(progName, "-o requires <output_path>");
		}
		else usage(progName, (((SWBuf)"Unknown argument: ")+ argv[i]).c_str());
	}
	// -----------------------------------------------------


	// setup module
	if (!append) {
		if (!fourByteSize)
			RawText::createModule(outPath);
		else	RawText4::createModule(outPath);
	}

	SWModule *module = (!fourByteSize)
			? (SWModule *)new RawText(outPath)
			: (SWModule *)new RawText4(outPath);
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

	delete vkey;

	return 0;
}





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
				std::cout << "adding entry: " << *vkey << " length " << entry.size() << "/" << (unsigned short)text.size() << std::endl;
				module->setEntry(text);
				first = false;
			}
			else {
				std::cout << "linking entry: " << *vkey << " to " << *linkMaster << std::endl;
				module->linkEntry(vkey);
			}
		}

		delete linkMaster;
	}
}
