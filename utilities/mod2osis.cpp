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
#include <markupfiltmgr.h>
#include <algorithm>

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

	SWMgr mgr(new MarkupFilterMgr(FMT_OSIS));
	OptionsList options = mgr.getGlobalOptions();
	for (OptionsList::iterator it = options.begin(); it != options.end(); it++) {
		OptionsList values = mgr.getGlobalOptionValues(it->c_str());
		if (find(values.begin(), values.end(), "On") != values.end()) {
			mgr.setGlobalOption(it->c_str(), "On");
		}
	}


	ModMap::iterator it = mgr.Modules.find(argv[1]);
	if (it == mgr.Modules.end()) {
		fprintf(stderr, "error: %s: couldn't find module: %s \n", argv[0], argv[1]);
		exit(-2);
	}

	inModule = it->second;
//	inModule->AddRenderFilter(&filter);

	SWKey *key = (SWKey *)*inModule;
	VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);

	char *buf;
	bool opendiv = false;
	VerseKey lastHeading;
	lastHeading.Headings(1);
	lastHeading.AutoNormalize(0);
	lastHeading.Testament(5);
	lastHeading = BOTTOM;

	if (vkey)
		vkey->Headings(1);

	cout << "<?xml version=\"1.0\" ";
		if (inModule->getConfigEntry("Encoding")) {
			if (*(inModule->getConfigEntry("Encoding")))
				cout << "encoding=\"" << inModule->getConfigEntry("Encoding") << "\" ";
			else cout << "encoding=\"UTF-8\" ";
		}
		else cout << "encoding=\"UTF-8\" ";
		cout << "?>\n\n";

	cout << "<text ";
		cout << "xmlns:xsi=\"http://www.w3.org/2000/10/XMLSchema-instance\" ";
		cout << "refSys=\"KJV\" ";
		cout << "work=\"";
		cout << inModule->Name() << "\" ";
		if (inModule->Lang()) {
			if (strlen(inModule->Lang()))
				cout << "lang=\"" << inModule->Lang() << "\" ";
		}
		cout << "xsi:noNamespaceSchemaLocation=\"OSISCore_1.0.2.xsd\" ";
		cout << " >\n\n";

	cout << "\t<front>\n";
	cout << "\t\t<title>\n";
	cout << "\t\t\t<titlePart>" << inModule->Description() << "</titlePart>\n";
	cout << "\t\t</title>\n";
	cout << "\t</front>\n\n";

	cout << "\t<body><div>\n";

	int testament = vkey->Testament();		
//	for ((*inModule) = TOP; (inModule->Key() < (VerseKey)"Mat 2:1"); (*inModule)++) {
	for ((*inModule) = TOP; !inModule->Error(); (*inModule)++) {
//	for ((*vkey) = "Mark6:29"; !inModule->Error(); (*inModule)++) {
		if (vkey->Testament() != lastHeading.Testament()) {
			if (opendiv)
				cout << "\t</div>\n";
			cout << "\t<div>";
			opendiv = true;
		}
		if (vkey->Book() != lastHeading.Book()) {
			buf = new char [205];
			lastHeading.Testament(vkey->Testament());
			lastHeading.Book(vkey->Book());
			lastHeading.Chapter(0);
			lastHeading.Verse(0);
			*buf = 0;
			filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
			cout << "" << buf << endl;
			delete [] buf;
		}
		if (vkey->Chapter() != lastHeading.Chapter()) {
			buf = new char [205];
			lastHeading.Testament(vkey->Testament());
			lastHeading.Book(vkey->Book());
			lastHeading.Chapter(vkey->Chapter());
			lastHeading.Verse(0);
			*buf = 0;
			filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
			cout << "" << buf;
			delete [] buf;
		}
		/*
		char *text = inModule->getRawEntry();
		int len = (inModule->getEntrySize() + 1) * 15;
		buf = new char [ len ];
		memset(buf, 0, len);
		memcpy(buf, inModule->getRawEntry(), inModule->getEntrySize());
		filter.ProcessText(buf,  len - 3, vkey, inModule);
		cout << buf << endl;
		*/
		cout << inModule->RenderText() << endl;
	}
	if (opendiv)
		cout << "\t</div>\n";
	cout << "\t</div></body>\n";
	cout << "\t</text>\n";
	return 0;
}

