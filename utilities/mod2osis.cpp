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
#include <zld.h>
#include <zcom.h>
#include <swmgr.h>
#include <lzsscomprs.h>
#include <zipcomprs.h>
#include <versekey.h>
#include <thmlosis.h>
#include <stdio.h>

void errorOutHelp(char *appName) {
	cerr << appName << " - a tool to output a Sword module in OSIS format\n";
	cerr << "usage: "<< appName << " <modname> \n";
	cerr << "\n\n";
	exit(-1);
}


int main(int argc, char **argv)
{
	SWModule *inModule = 0;
	ThMLOSIS filter;
	
	if ((argc != 2)) {
		errorOutHelp(argv[0]);
	}

	if ((!strcmp(argv[1], "-h")) || (!strcmp(argv[1], "--help")) || (!strcmp(argv[1], "/?")) || (!strcmp(argv[1], "-?")) || (!strcmp(argv[1], "-help"))) {
		errorOutHelp(argv[0]);
	}

	SWMgr mgr;

	ModMap::iterator it = mgr.Modules.find(argv[1]);
	if (it == mgr.Modules.end()) {
		fprintf(stderr, "error: %s: couldn't find module: %s \n", argv[0], argv[1]);
		exit(-2);
	}

	inModule = it->second;
	inModule->AddRenderFilter(&filter);

	SWKey *key = (SWKey *)*inModule;
	VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);

	char buf[200];
	VerseKey lastHeading;
	lastHeading.Headings(1);
	lastHeading = BOTTOM;

	if (vkey)
		vkey->Headings(1);

	for ((*inModule) = TOP; !inModule->Error(); (*inModule)++) {
		if (vkey->Testament() != lastHeading.Testament()) {
			lastHeading.Testament(vkey->Testament());
			lastHeading.Book(0);
			lastHeading.Chapter(0);
			lastHeading.Verse(0);
			*buf = 0;
			filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
			cout << buf;
		}
		if (vkey->Book() != lastHeading.Book()) {
			lastHeading.Book(vkey->Book());
			lastHeading.Chapter(0);
			lastHeading.Verse(0);
			*buf = 0;
			filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
			cout << buf;
		}
		if (vkey->Chapter() != lastHeading.Chapter()) {
			lastHeading.Chapter(vkey->Chapter());
			lastHeading.Verse(0);
			*buf = 0;
			filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
			cout << buf;
		}
		cout << inModule->RenderText() << endl;
	}
	return 0;
}

