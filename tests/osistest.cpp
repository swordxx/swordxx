#include <iostream>

#include <swmgr.h>
#include <markupfiltmgr.h>
#include <swmodule.h>

using namespace std;
using namespace sword;


void outputCurrentVerse(SWModule *module) {

	module->RenderText();

	cout << "Key:\n";
	cout << module->getKeyText() << "\n";
	cout << "-------\n";

	cout << "Preverse Header 0:\nRaw:\n";
	SWBuf header = module->getEntryAttributes()["Heading"]["Preverse"]["0"];
	cout << header << endl;
	cout << "-------\n";
	cout << "Rendered Header:\n";
	cout << module->RenderText(header) << endl;
	cout << "-------\n";
	cout << "CSS:\n";
	cout << module->getRenderHeader() << endl;
	cout << "-------\n";
	cout << "RenderText:\n";
	cout << module->RenderText() << endl;
	cout << "-------\n";
	cout << "-------\n\n";
}


int main(int argc, char **argv) {

	if (argc != 2) {
		cerr << "\nusage: " << *argv << " <modName>\n" << endl;
		exit(-1);
	}

	SWMgr library(new MarkupFilterMgr(FMT_XHTML));
	library.setGlobalOption("Headings", "On");

	SWModule *module = library.getModule(argv[1]);

	if (!module) {
		cerr << "\nCouldn't file modules: " << argv[1] << "\n" << endl;
		exit(-2);
	}

	module->setKey("Ps.3.1");
	outputCurrentVerse(module);

	module->setKey("Mark.1.14");
	outputCurrentVerse(module);

	return 0;
}
