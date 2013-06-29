/******************************************************************************
 *
 *  imp2ld.cpp -	Utility to import LD modules in IMP format
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifdef _MSC_VER
	#pragma warning( disable: 4251 )
#endif

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <rawld.h>
#include <rawld4.h>
#include <zld.h>
#include <zipcomprs.h>
#include <lzsscomprs.h>
#include <stdio.h>

using std::string;

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

void usage(const char *progName, const char *error = 0) {
	if (error) fprintf(stderr, "\n%s: %s\n", progName, error);
	fprintf(stderr, "\n=== imp2ld (Revision $Rev: 2234 $) SWORD lexicon importer.\n");
	fprintf(stderr, "\nusage: %s <imp_file> [options]\n", progName);
	fprintf(stderr, "  -a\t\t\t augment module if exists (default is to create new)\n");
	fprintf(stderr, "  -z\t\t\t use ZIP compression (default no compression)\n");
	fprintf(stderr, "  -Z\t\t\t use LZSS compression (default no compression)\n");
	fprintf(stderr, "  -o <output_path>\t where to write data files.\n");
	fprintf(stderr, "  -4\t\t\t use 4 byte size entries (default is 2).\n");
	fprintf(stderr, "  -b <entry_count>\t\t compression block size (default 30 entries)\n");
	fprintf(stderr, "  -s\t\t\t case sensitive keys (default is not case sensitive)\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "'imp' format is a simple standard for importing data into SWORD modules.\n"
		"Required is a plain text file containing $$$key lines followed by content.\n\n"
		"$$$Abraham\n"
		"Abraham was the father of Isaac...\n"
		"He was called by God to leave his country and journey to the land of Canaan...\n"
		"$$$Isaac\n"
		"Isaac was the son of Abraham and Sarah...\n\n");
	exit(-1);
}

int main(int argc, char **argv) {

	std::vector<string> linkbuffer;
	signed long i = 0;
	string keybuffer;
	string entbuffer;
	string linebuffer;
	char links = 0;
	string modname;
	SWBuf outPath          = "";
	bool append            = false;
	long blockCount = 30;
	bool caseSensitive = false;
	SWCompress *compressor = 0;
	SWBuf compType         = "";
	bool fourByteSize      = false;

	if (argc < 2) usage(*argv);

	const char *progName   = argv[0];
	const char *inFileName = argv[1];

	for (int i = 2; i < argc; i++) {
		if (!strcmp(argv[i], "-a")) {
			append = true;
		}
		else if (!strcmp(argv[i], "-z")) {
			if (compType.size()) usage(*argv, "Cannot specify both -z and -Z");
			if (fourByteSize) usage(*argv, "Cannot specify both -z and -4");
			compType = "ZIP";
		}
		else if (!strcmp(argv[i], "-Z")) {
			if (compType.size()) usage(*argv, "Cannot specify both -z and -Z");
			if (fourByteSize) usage(*argv, "Cannot specify both -Z and -4");
			compType = "LZSS";
		}
		else if (!strcmp(argv[i], "-4")) {
			fourByteSize = true;
		}
		else if (!strcmp(argv[i], "-b")) {
			if (i+1 < argc) {
				blockCount = atoi(argv[++i]);
				if (blockCount > 0) continue;
			}
			usage(*argv, "-b requires in entry count integer > 0");
		}
		else if (!strcmp(argv[i], "-o")) {
			if (i+1 < argc) outPath = argv[++i];
			else usage(progName, "-o requires <output_path>");
		}
		else if (!strcmp(argv[i], "-s")) {
			caseSensitive = true;
		}
		else usage(progName, (((SWBuf)"Unknown argument: ")+ argv[i]).c_str());
	}



	if (outPath.size() < 1) {
		for (i = 0; (i < 16) && (inFileName[i]) && (inFileName[i] != '.'); i++) {
			outPath += inFileName[i];
		}
	}

	std::ifstream infile(inFileName);


	SWModule *mod = 0;
	SWKey *key, *linkKey;

	if (compType == "ZIP") {
#ifndef EXCLUDEZLIB
		compressor = new ZipCompress();
#else
		usage(*argv, "ERROR: SWORD library not compiled with ZIP compression support.\n\tBe sure libzip is available when compiling SWORD library");
#endif
	}
	else if (compType == "LZSS") {
		compressor = new LZSSCompress();
	}

	// setup module
	if (!append) {
		if (compressor) {
			if (zLD::createModule(outPath)) {
				fprintf(stderr, "ERROR: %s: couldn't create module at path: %s \n", *argv, outPath.c_str());
				exit(-1);
			}
		}
		else {
			if (!fourByteSize)
				RawLD::createModule(outPath);
			else	RawLD4::createModule(outPath);
		}
	}

	if (compressor) {
		// Create a compressed text module allowing very large entries
		// Taking defaults except for first, fourth, fifth and last argument
		mod = new zLD(outPath, 0, 0, blockCount, compressor, 0, ENC_UNKNOWN, DIRECTION_LTR, FMT_UNKNOWN, 0, caseSensitive);
	}
	else {
		mod = (!fourByteSize)
			? (SWModule *)new RawLD (outPath, 0, 0, 0, ENC_UNKNOWN, DIRECTION_LTR, FMT_UNKNOWN, 0, caseSensitive)
			: (SWModule *)new RawLD4(outPath, 0, 0, 0, ENC_UNKNOWN, DIRECTION_LTR, FMT_UNKNOWN, 0, caseSensitive);
	}




	key = mod->createKey();
	linkKey = mod->createKey();
	key->setPersist(true);
	mod->setKey(key);

	while (!infile.eof()) {
		std::getline(infile, linebuffer);
		if (linebuffer.size() > 3 && linebuffer.substr(0,3) == "$$$") {
			if (keybuffer.size() && entbuffer.size()) {
				std::cout << keybuffer << std::endl;
				*key = keybuffer.c_str();

				mod->setEntry(entbuffer.c_str(), entbuffer.size());
				for (i = 0; i < links; i++) {
					std::cout << "Linking: " << linkbuffer[i] << std::endl;
					*linkKey = linkbuffer[i].c_str();
					mod->linkEntry(linkKey);
				}
			}
			if (linebuffer.size() > 3)
				keybuffer = linebuffer.substr(3,linebuffer.size());

			entbuffer.resize(0);
			linkbuffer.clear();
			links = 0;
		}
		else if (linebuffer.size() > 3 && linebuffer.substr(0,3) == "%%%") {
			linkbuffer.push_back(linebuffer.substr(3,linebuffer.size()));
			links++;
		}
		else {
			entbuffer += linebuffer;
		}
	}

	//handle final entry
	if (keybuffer.size() && entbuffer.size()) {
		std::cout << keybuffer << std::endl;
		*key = keybuffer.c_str();

		mod->setEntry(entbuffer.c_str(), entbuffer.size());
		for (i = 0; i < links; i++) {
			std::cout << "Linking: " << linkbuffer[i] << std::endl;
			*linkKey = linkbuffer[i].c_str();
			mod->linkEntry(linkKey);
		}
	}

	infile.close();

	delete linkKey;
	delete key;
	delete mod;

	return 0;
}
