// Compression on variable granularity

#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <ztext.h>
#include <swmgr.h>
#include <lzsscomprs.h>
#include <zipcomprs.h>
#include <versekey.h>
#include <stdio.h>


void errorOutHelp(char *appName) {
	cerr << appName << " - a tool to create compressed Sword modules\n";
	cerr << "version 0.1\n\n";
	cerr << "usage: "<< appName << " <modname> <datapath> [blockType [compressType]]\n\n";
	cerr << "datapath: the directory in which to write the zModule\n";
	cerr << "blockType  : (default 4)\n\t2 - verses\n\t3 - chapters\n\t4 - books\n";
	cerr << "compressType: (default 1):\n\t1 - LZSS\n\t2 - Zip\n";
	cerr << "\n\n";
	exit(-1);
}


int main(int argc, char **argv)
{
	int iType = 4;
	int compType = 1;
	SWCompress *compressor = 0;

	if ((argc < 3) || (argc > 5)) {
		errorOutHelp(argv[0]);
	}

	if (argc > 3) {
		iType = atoi(argv[3]);
		if (argc > 4) {
			compType = atoi(argv[4]);
		}
	}

	if ((iType < 2) || (iType > 4) || (compType < 1) || (compType > 2) || (!strcmp(argv[1], "-h")) || (!strcmp(argv[1], "--help")) || (!strcmp(argv[1], "/?")) || (!strcmp(argv[1], "-?")) || (!strcmp(argv[1], "-help"))) {
		errorOutHelp(argv[0]);
	}

	SWMgr mgr;

	ModMap::iterator it = mgr.Modules.find(argv[1]);
	if (it == mgr.Modules.end()) {
		fprintf(stderr, "error: %s: couldn't find module: %s \n", argv[0], argv[1]);
		exit(-2);
	}

	// Try to initialize a default set of datafiles and indicies at our
	// datapath location passed to us from the user.
	if (zText::createModule(argv[2], iType)) {
		fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
		exit(-3);
	}

	switch (compType) {	// these are deleted by zText
	case 1: compressor = new LZSSCompress(); break;
	case 2: compressor = new ZipCompress(); break;
	}

	zText outModule(argv[2], 0, 0, iType, compressor);	// open our datapath with our RawText driver.
	SWModule *inModule = it->second;

	SWKey *key = inModule->CreateKey();
	VerseKey *vkey = 0;
	try {
		vkey = dynamic_cast<VerseKey *>(key);
	}
	catch (...) {}

	if (!vkey) {
		fprintf(stderr, "error: %s: %s inModule is not keyed to verses \n", argv[0], argv[1]);
		exit(-3);
	}

	vkey->Headings(1);	// turn on mod/testmnt/book/chap headings
	vkey->Persist(1);

	inModule->SetKey(*vkey);
	outModule.SetKey(*vkey);


	char *lastBuffer = new char [55];
	strcpy(lastBuffer, "Something that would never be first module entry");
	VerseKey bufferKey;

	(*vkey) = TOP;
	while (!vkey->Error()) {
		cout << "Adding [" << *vkey << "]\n";
		// pseudo-check for link.  Will get most common links.
		if (!strcmp(lastBuffer, (const char *)(*inModule))) {
			outModule << bufferKey;	// link to last key
		}
		else {
			delete [] lastBuffer;
			lastBuffer = new char [ strlen (inModule->getRawEntry()) + 1 ];
			strcpy(lastBuffer, inModule->getRawEntry());
			bufferKey = *vkey;

			outModule << lastBuffer;	// save new text;
		}
		(*vkey)++;
	}

	// clear up our buffer that readline might have allocated
	if (lastBuffer)
		delete [] lastBuffer;
}
