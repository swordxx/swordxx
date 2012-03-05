#include <iostream>

#include <swmgr.h>
#include <markupfiltmgr.h>
#include <swmodule.h>

using namespace std;
using namespace sword;


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

	module->RenderText();

	SWBuf header = module->getEntryAttributes()["Heading"]["Preverse"]["0"];

	cout << "-------\n";
	cout << header << endl;

	cout << "-------\n";
	cout << module->RenderText(header) << endl;

	cout << "-------\n";
	cout << module->getRenderHeader() << endl;
	cout << module->RenderText() << endl;

	return 0;
}
