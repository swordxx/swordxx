// Compression on variable granularity

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

#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

//#include <ztext.h>
#include <ztext2.h>
//#include <zld.h>
//#include <zcom.h>
#include <swmgr.h>
#include <lzsscomprs.h>
#include <zipcomprs.h>
#include <versekey.h>
#include <versekey2.h>
#include <stdio.h>
#include <cipherfil.h>
#ifndef NO_SWORD_NAMESPACE
using sword::SWCompress;
using sword::CipherFilter;
using sword::SWModule;
using sword::SWMgr;
using sword::ModMap;
using sword::zText2;
//using sword::zLD;
//using sword::zCom;
using sword::SWFilter;
using sword::VerseKey2;
using sword::VerseKey;
using sword::SWKey;
using sword::SW_POSITION;
using sword::ZipCompress;
using sword::LZSSCompress;
#endif

using std::cerr;
using std::string;
using std::cout;
using std::endl;

void errorOutHelp(char *appName) {
	cerr << appName << " - a tool to create compressed Sword modules\n";
	cerr << "version 0.1\n\n";
	cerr << "usage: "<< appName << " <modname> <datapath> [blockType [blockNum [compressType [cipherKey]]]]\n\n";
	cerr << "datapath: the directory in which to write the zModule\n";
	cerr << "blockType  : (default 4)\n\t2 - verses\n\t3 - chapters\n\t4 - books\n";
	cerr << "blockNum  : (default 1)\n\tnumber of blockType per block\n";
	cerr << "compressType: (default 1):\n\t1 - LZSS\n\t2 - Zip\n";
	cerr << "\n\n";
	exit(-1);
}


int main(int argc, char **argv)
{
	int iType = 4;
	int iPerBlock = 1;
	int compType = 1;
	string cipherKey = "";
	SWCompress *compressor = 0;
	SWModule *inModule     = 0;
	SWModule *outModule    = 0;
	

	if ((argc < 3) || (argc > 6)) {
		errorOutHelp(argv[0]);
	}

	if (argc > 3) {
		iType = atoi(argv[3]);
		if (argc > 4) {
			iPerBlock = atoi(argv[4]);
			if (argc > 5) {
				compType = atoi(argv[5]);
				if (argc > 6) {
					cipherKey = argv[6];
				}
			}
		}
	}

	if ((iType < 2) || (iType > 5) || (iPerBlock < 1) || (compType < 1) || (compType > 2) || (!strcmp(argv[1], "-h")) || 
		(!strcmp(argv[1], "--help")) || (!strcmp(argv[1], "/?")) || (!strcmp(argv[1], "-?")) || (!strcmp(argv[1], "-help"))) {
		errorOutHelp(argv[0]);
	}

	SWMgr mgr;

	ModMap::iterator it = mgr.Modules.find(argv[1]);
	if (it == mgr.Modules.end()) {
		fprintf(stderr, "error: %s: couldn't find module: %s \n", argv[0], argv[1]);
		exit(-2);
	}

	inModule = it->second;

	// Try to initialize a default set of datafiles and indicies at our
	// datapath location passed to us from the user.
	
#define BIBLE 1
#define LEX 2
#define COM 3

	int modType = 0;
	if (!strcmp(inModule->Type(), "Biblical Texts")) modType = BIBLE;
	if (!strcmp(inModule->Type(), "Lexicons / Dictionaries")) modType = LEX;
	if (!strcmp(inModule->Type(), "Commentaries")) modType = COM;

	switch (compType) {	// these are deleted by zText
	case 1: compressor = new LZSSCompress(); break;
	case 2: compressor = new ZipCompress(); break;
	}
	cout << "Creating module " << argv[2] << endl;
	int result = 1;
	switch (modType) {
	case BIBLE:
		result = zText2::createModule(argv[2], iType);
		break;
	case LEX:
		//result = zLD::createModule(argv[2]);
		result = 1;
		break;
	case COM:
		result = zText2::createModule(argv[2], iType);
		break;
	}

	if (result) {
		fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
		exit(-3);
	}

	cout << "Opening datapath with driver " << argv[2] << " Type " << iType << " PerBlock " << iPerBlock << endl;
	switch (modType) {
	case BIBLE:
		outModule = new zText2(argv[2], 0, 0, iType, iPerBlock, compressor);	// open our datapath with our RawText driver.
		((VerseKey *)(SWKey *)(*inModule))->Headings(1);
		break;
	case LEX:
		//outModule = new zLD(argv[2], 0, 0, iType, compressor);	// open our datapath with our RawText driver.
		break;
	case COM:
		outModule = new zText2(argv[2], 0, 0, iType, iPerBlock, compressor);	// open our datapath with our RawText driver.
		((VerseKey *)(SWKey *)(*inModule))->Headings(1);
		break;
	}

	SWFilter *cipherFilter = 0;
	if (!cipherKey.empty()) {
		cipherFilter = new CipherFilter(cipherKey.c_str());
		outModule->AddRawFilter(cipherFilter);
	}

	string lastBuffer = "Something that would never be first module entry";
	SWKey bufferKey;
	SWKey lastBufferKey;
	SWKey *outModuleKey = outModule->CreateKey();
	VerseKey2 *vkey = SWDYNAMIC_CAST(VerseKey2, outModuleKey);
	outModuleKey->Persist(1);
	if (vkey) {
		vkey->Headings(1);
		vkey->AutoNormalize(0);
	}
	outModule->setKey(*outModuleKey);

	inModule->setSkipConsecutiveLinks(false);
	(*inModule) = TOP;
	while (!inModule->Error()) {
		bufferKey = *(SWKey *)(*inModule);
		cout << bufferKey << endl;
		// pseudo-check for link.  Will get most common links.
		if ((lastBuffer == inModule->getRawEntry()) &&(lastBuffer.length() > 0)) {
			*outModuleKey = bufferKey;
			outModule->linkEntry(&lastBufferKey);	// link to last key
		//cout << "Adding [" << bufferKey << "] link to: [" << lastBufferKey << "]\n";
		}
		else {
			lastBuffer = inModule->getRawEntry();
			lastBufferKey = inModule->KeyText();
			if (lastBuffer.length() > 0) {
				//cout << "Adding [" << bufferKey << "] new text. \n";
				*outModuleKey = bufferKey;
//				outModule->getRawEntry();	// snap
//				outModule->setKey(bufferKey);
				(*outModule) << lastBuffer.c_str();	// save new text;
			}
			else {
				cout << "Skipping [" << bufferKey << "] no entry in inModule. \n";
			}
		}
		(*inModule)++;
	}
	delete outModule;
	delete outModuleKey;
	if (cipherFilter)
		delete cipherFilter;
}

