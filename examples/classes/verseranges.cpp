#include <iostream>

#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {

	cout << "\n";

	const char *modName = "KJVA";
	SWMgr manager;
	SWModule *bible = manager.getModule(modName);
	if (!bible) {
		cout << modName << " not installed for example.  Please install.\n\n";
		exit(-1);
	}

	VerseKey *vk = (VerseKey *)bible->CreateKey();


	// whole Bible
	vk->LowerBound(*vk);
	vk->setPosition(BOTTOM);
	vk->UpperBound(*vk);
	cout << vk->getRangeText() << "\n";


	vk->ClearBounds();


	// Old Testament
	vk->setPosition(TOP);
	vk->LowerBound(*vk);
	// doesn't work because MAXBOOK hasn't been implemented in versekey.cpp:1159
//	vk->setPosition(MAXBOOK); vk->setPosition(MAXCHAPTER); vk->setPosition(MAXVERSE);
	vk->setTestament(2);
	(*vk)--;
//	---- end of workaround
	vk->UpperBound(*vk);
	cout << vk->getRangeText() << "\n";


	vk->ClearBounds();


	// New Testament
	vk->setPosition(TOP);	// stupid workaround to set book, chap, and verse to 1 because setTestament doesn't follow suit and do this like setChapter and setBook do.
	vk->setTestament(2);
	vk->LowerBound(*vk);
	vk->setPosition(BOTTOM);
	vk->UpperBound(*vk);
	cout << vk->getRangeText() << "\n";


	vk->ClearBounds();


	// Current Book
	vk->setText("John 3:16");
	vk->setChapter(1); vk->setVerse(1);
	vk->LowerBound(*vk);
	vk->setChapter(vk->getChapterMax()); vk->setVerse(vk->getVerseMax());
	vk->UpperBound(*vk);
	cout << vk->getRangeText() << "\n";


	// Shorter syntax using the parser and based on book names
	const VerseMgr::System *refSys = VerseMgr::getSystemVerseMgr()->getVersificationSystem(vk->getVersificationSystem());


	// whole Bible
	VerseKey vkBible(refSys->getBook(0)->getOSISName(), refSys->getBook(refSys->getBookCount()-1)->getOSISName(), refSys->getName());
	cout << vkBible.getRangeText() << "\n";

	// OT
	VerseKey vkOT(refSys->getBook(0)->getOSISName(), refSys->getBook(refSys->getBMAX()[0]-1)->getOSISName(), refSys->getName());
	cout << vkOT.getRangeText() << "\n";

	// NT
	VerseKey vkNT(refSys->getBook(refSys->getBMAX()[0])->getOSISName(), refSys->getBook(refSys->getBookCount()-1)->getOSISName(), refSys->getName());
	cout << vkNT.getRangeText() << "\n";

	// Current Book
	vk->setText("John 3:16");
	VerseKey vkCurrentBook(vk->getBookName(), vk->getBookName(), refSys->getName());
	cout << vkCurrentBook.getRangeText() << "\n";


	delete vk;

	cout << endl;

	return 0;
}
