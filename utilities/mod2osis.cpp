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

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using std::endl;
using std::cerr;
using std::cout;

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
	bool opentest = false;
	bool openbook = false;
	bool openchap = false;
	VerseKey lastHeading;
	lastHeading.Headings(1);
	lastHeading.AutoNormalize(0);
	lastHeading.Testament(5);
	lastHeading = BOTTOM;

	if (!vkey) {
		cerr << "Currently mod2zmod only works with verse keyed modules\n\n";
		exit(-1);
	}

	vkey->Headings(1);

	cout << "<?xml version=\"1.0\" ";
		if (inModule->getConfigEntry("Encoding")) {
			if (*(inModule->getConfigEntry("Encoding")))
				cout << "encoding=\"" << inModule->getConfigEntry("Encoding") << "\" ";
			else cout << "encoding=\"UTF-8\" ";
		}
		else cout << "encoding=\"UTF-8\" ";
		cout << "?>\n\n";


	cout << "<osis";
		cout << " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"";
		cout << " xsi:noNamespaceSchemaLocation=\"osisCore.1.1.xsd\">\n\n";
	cout << "<osisText";
		cout << " osisIDWork=\"";
		cout << inModule->Name() << "\"";
		cout << " osisRefWork=\"defaultReferenceScheme\"";
		cout << ">\n\n";

	cout << "\t<header>\n";
	cout << "\t\t<work osisWork=\"";
	cout << inModule->Name() << "\">\n";
	cout << "\t\t\t<title>" << inModule->Description() << "</title>\n";
	cout << "\t\t\t<identifier type=\"OSIS\">" << inModule->Name() << "</identifier>\n";
	if (inModule->Lang()) {
		if (strlen(inModule->Lang()))
			cout << "\t\t\t<language>" << inModule->Lang() << "</language>\n";
	}
	cout << "\t\t\t<refSystem>Bible.KJV</refSystem>\n";
	cout << "\t\t</work>\n";
	cout << "\t\t<work osisWork=\"defaultReferenceScheme\">\n";
	cout << "\t\t\t<refSystem>Bible.KJV</refSystem>\n";
	cout << "\t\t</work>\n";
	cout << "\t</header>\n\n";


	(*inModule) = TOP; 
	int testament = vkey->Testament();		
//	for ((*inModule) = TOP; (inModule->Key() < (VerseKey)"Mat 2:1"); (*inModule)++) {
	for ((*inModule) = TOP; !inModule->Error(); (*inModule)++) {
//	for ((*vkey) = "Mark6:29"; !inModule->Error(); (*inModule)++) {
		if (vkey->Testament() != lastHeading.Testament()) {
			if (opentest)
				cout << "\t</div>\n";
			cout << "\t<div type=\"testament\">";
			lastHeading = *vkey;
			lastHeading.Book(0);
			lastHeading.Chapter(0);
			lastHeading.Verse(0);
			opentest = true;
		}
		if (vkey->Book() != lastHeading.Book()) {
			buf = new char [205];
			lastHeading = *vkey;
			lastHeading.Chapter(0);
			lastHeading.Verse(0);
			*buf = 0;
			filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
			cout << "" << buf << endl;
			delete [] buf;
			openbook = true;
		}
		if (vkey->Chapter() != lastHeading.Chapter()) {
			if (openchap)
				cout << "\t</div>\n";
			buf = new char [205];
			lastHeading = *vkey;
			lastHeading.Verse(0);
			*buf = 0;
			filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
			cout << "" << buf;
			delete [] buf;
			openbook = true;
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
	if (opentest)
		cout << "\t</div>\n";
	cout << "\t</osisText>\n";
	cout << "</osis>\n";
	return 0;
}


