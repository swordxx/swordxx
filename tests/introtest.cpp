#include <swmgr.h>
#include <iostream>
#include <versekey.h>
#include <rawtext.h>

int main(int argc, char **argv) {
	SWMgr mymgr;

	RawText::createModule(".");
	RawText mod(".");

	VerseKey vk;
	vk.Headings(1);
	vk.AutoNormalize(0);
	vk.Persist(1);
	mod.setKey(vk);

	vk.Verse(0);
	vk.Chapter(0);
	vk.Book(0);
	vk.Testament(0);

	mod << "Module heading text";

	vk.Verse(0);
	vk.Chapter(0);
	vk.Book(0);
	vk.Testament(1);

	mod << "OT heading text";

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(0);
	vk.Verse(0);

	mod << "Gen heading text";

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(1);
	vk.Verse(0);

	mod << "Gen 1 heading text";

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(1);
	vk.Verse(1);

	mod << "Gen 1:1 text";

	
	vk.Testament(0);
	vk.Book(0);
	vk.Chapter(0);
	vk.Verse(0);

	std::cout << "Module heading text ?= " << (const char*)mod << std::endl;

	vk.Testament(1);
	vk.Book(0);
	vk.Chapter(0);
	vk.Verse(0);

	std::cout << "OT heading text ?= " << (const char*)mod << std::endl;

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(0);
	vk.Verse(0);

	std::cout << "Gen heading text ?= " << (const char*)mod << std::endl;

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(1);
	vk.Verse(0);

	std::cout << "Gen 1 heading text ?= " << (const char*)mod << std::endl;

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(1);
	vk.Verse(1);

	std::cout << "Gen 1:1 text ?= " << (const char*)mod << std::endl;

	  /* old introtest
	SWModule *mhc = mymgr.Modules["MHC"];

	if (mhc) {
		VerseKey vk;
		vk.Headings(1);
		vk.AutoNormalize(0);
		vk.Persist(1);
		vk = "jas 0:0";
		std::cout << vk << ":\n";
		mhc->setKey(vk);
		std::cout << (const char *) mhc->Key() << ":\n";
		std::cout << (const char *) *mhc << "\n";
	}
	  */
	return 0;
}


