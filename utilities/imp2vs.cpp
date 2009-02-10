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
	fprintf(stderr, "%s (Revision $Rev: 2234 $) SWORD Bible/Commentary importer.\n", progName);
	fprintf(stderr, "usage: %s <imp_file> [options]\n"
		"\t -a augment module if exists (default is to create new)\n"
		"\t -o <output_path> where to write data files.\n"
		"\t -4 use 4 byte size entries (default is 2).\n"
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

	return 0;
}





void writeEntry(const SWBuf &key, const SWBuf &entry, SWModule *module)
{
	if (key.size() && entry.size()) {
		std::cout << "from file: " << key << std::endl;
		VerseKey *vkey = (VerseKey *)module->getKey();
		*vkey = key;
		if (!vkey->Chapter()) {
			// bad hack:  0:0 is Book intro; (chapter):0 is Chapter intro; 0:2 is Module intro; 0:1 is Testament intro
			int backstep = vkey->Verse();
			if (backstep) {
				vkey->Verse(1);
				vkey->Chapter(1);
				switch (backstep) {
				case 2:
					vkey->Book(1);
					vkey->Testament(0);
				case 1:
					vkey->Book(0);
					vkey->Chapter(0);
				}
				vkey->Verse(0);
			}

			std::cout << "adding entry: " << *vkey << " length " << entry.size() << "/" << (unsigned short)entry.size() << std::endl;
			module->setEntry(entry.c_str(), entry.size());
		}
		else {
			ListKey listkey = vkey->ParseVerseList(key.c_str(), "Gen1:1", true);
			int i;
			bool havefirst = false;
			VerseKey firstverse;
			firstverse.Headings(1);
			firstverse.AutoNormalize(0);
			for (i = 0; i < listkey.Count(); i++) {
				VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.GetElement(i));
				if (element) {
					*vkey = element->LowerBound();
					VerseKey finalkey = element->UpperBound();
					finalkey.Headings(1);
					finalkey.AutoNormalize(0);
					if (!havefirst) {
						havefirst = true;
						firstverse = *vkey;

						std::cout << "adding entry: " << *vkey << " length " << entry.size() << "/" << (unsigned short)entry.size() << std::endl;
						module->setEntry(entry.c_str(), entry.size());
						(*vkey)++;
					}
					while (! (finalkey < (*vkey))) {
						std::cout << "linking entry: " << *vkey << " to " << firstverse << std::endl;
						*module << &firstverse;
						(*vkey)++;
					}
				}
				else {
					if (havefirst) {
						*vkey = (*listkey.GetElement(i));
						std::cout << "linking entry: " << *vkey << " to " << firstverse << std::endl;
						*module << &firstverse;
					}
					else {
						*vkey = (*listkey.GetElement(i));
						havefirst = true;
						firstverse = *vkey;

						std::cout << "adding entry: " << *vkey << " length " << entry.size() << "/" << (unsigned short)entry.size() << std::endl;
						module->setEntry(entry.c_str(), entry.size());
					}
				}
			}
		}
	}
}
