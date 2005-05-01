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
	StringList options = mgr.getGlobalOptions();
	for (StringList::iterator it = options.begin(); it != options.end(); it++) {
		StringList values = mgr.getGlobalOptionValues(it->c_str());
		if (find(values.begin(), values.end(), "On") != values.end()) {
			mgr.setGlobalOption(it->c_str(), "On");
		}
	}

//	mgr.setGlobalOption("Strong's Numbers", "Off");
//	mgr.setGlobalOption("Morphological Tags", "Off");

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
	int lastTest = 5;
	int lastBook = 9999;
	int lastChap = 9999;
	if (!vkey) {
		cerr << "Currently mod2zmod only works with verse keyed modules\n\n";
		exit(-1);
	}

	vkey->Headings(0);

	cout << "<?xml version=\"1.0\" ";
		if (inModule->getConfigEntry("Encoding")) {
			if (*(inModule->getConfigEntry("Encoding")))
				cout << "encoding=\"" << inModule->getConfigEntry("Encoding") << "\" ";
			else cout << "encoding=\"UTF-8\" ";
		}
		else cout << "encoding=\"UTF-8\" ";
		cout << "?>\n\n";


	cout << "<osis";
		cout << " xmlns=\"http://www.bibletechnologies.net/2003/OSIS/namespace\"";
		cout << " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"";
		cout << " xsi:schemaLocation=\"http://www.bibletechnologies.net/2003/OSIS/namespace osisCore.2.5.xsd\">\n\n";
	cout << "<osisText";
		cout << " osisIDWork=\"";
		cout << inModule->Name() << "\"";
		cout << " osisRefWork=\"defaultReferenceScheme\"";
		if (inModule->Lang()) {
			if (strlen(inModule->Lang()))
				cout << " xml:lang=\"" << inModule->Lang() << "\"";
		}
		cout << ">\n\n";

	cout << "\t<header>\n";
	cout << "\t\t<work osisWork=\"";
	cout << inModule->Name() << "\">\n";
	cout << "\t\t\t<title>" << inModule->Description() << "</title>\n";
	cout << "\t\t\t<identifier type=\"OSIS\">Bible." << inModule->Name() << "</identifier>\n";
	cout << "\t\t\t<refSystem>Bible.KJV</refSystem>\n";
	cout << "\t\t</work>\n";
	cout << "\t\t<work osisWork=\"defaultReferenceScheme\">\n";
	cout << "\t\t\t<refSystem>Bible.KJV</refSystem>\n";
	cout << "\t\t</work>\n";
	cout << "\t</header>\n\n";


	(*inModule) = TOP; 
//	for ((*inModule) = TOP; (inModule->Key() < (VerseKey)"Mat 2:1"); (*inModule)++) {
//	for ((*vkey) = "Mark6:29"; !inModule->Error(); (*inModule)++) {

	VerseKey tmpKey;
	tmpKey.Headings(1);
	tmpKey.AutoNormalize(0);

	for ((*inModule) = TOP; !inModule->Error(); (*inModule)++) {
		bool newTest = false;
		bool newBook = false;

		if (!strlen(inModule->RenderText())) {
			continue;
		}

		if ((vkey->Testament() != lastTest)) {
			if (openchap)
				cout << "\t</chapter>\n";
			if (openbook)
				cout << "\t</div>\n";
			if (opentest)
				cout << "\t</div>\n";
			cout << "\t<div type=\"x-testament\">\n";
			opentest = true;
			newTest = true;
		}
		if ((vkey->Book() != lastBook) || newTest) {
			if (!newTest) {
				if (openchap)
					cout << "\t</chapter>\n";
				if (openbook)
					cout << "\t</div>\n";
			}
			buf = new char [205];
			*buf = 0;
			tmpKey = *vkey;
			tmpKey.Chapter(0);
			tmpKey.Verse(0);
			sprintf(buf, "\t<div type=\"book\" osisID=\"%s\">\n", tmpKey.getOSISRef());
//			filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
			cout << "" << buf << endl;
			delete [] buf;
			openbook = true;
			newBook = true;
		}
		if ((vkey->Chapter() != lastChap) || newBook) {
			if (!newBook) {
				if (openchap)
					cout << "\t</chapter>\n";
			}
			buf = new char [205];
			*buf = 0;
			tmpKey = *vkey;
			tmpKey.Verse(0);
			sprintf(buf, "\t<chapter osisID=\"%s\">\n", tmpKey.getOSISRef());
//			filter.ProcessText(buf, 200 - 3, &lastHeading, inModule);
			cout << "" << buf;
			delete [] buf;
			openchap = true;
		}
		SWBuf verseText = inModule->RenderText();
		cout << verseText.c_str() << endl;
		lastChap = vkey->Chapter();
		lastBook = vkey->Book();
		lastTest = vkey->Testament();
	}
	if (openchap)
		cout << "\t</chapter>\n";
	if (openbook)
		cout << "\t</div>\n";
	if (opentest)
		cout << "\t</div>\n";
	cout << "\t</osisText>\n";
	cout << "</osis>\n";
	return 0;
}


