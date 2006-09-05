/******************************************************************************
 * This is about the simplest useful example of using the sword engine.
 *
 * After sword is installed, it should compile with something similar to:
 *
 *	g++ -o simple simple.cpp `pkg-config --cflags --libs sword`
 *
 * If you'd like to choose the type of tag markup which sword will output
 *	for you, include:
 *
 *	#include <markupfiltmgr.h>
 *
 * and change your instantiation of SWMgr, below, to (e.g., for HTML):
 *
 *	SWMgr library(0, 0, true, new MarkupFilterMgr(FMT_HTMLHREF));
 *
 */

#include <swmgr.h>
#include <swmodule.h>
#include <iostream>

using namespace sword;
using std::cout;

int main(int argc, char **argv) {
	SWMgr library;
	SWModule *darby = library.getModule("Darby");
	darby->setKey("James 1:19");
	cout << darby->RenderText();

	return 0;
}
