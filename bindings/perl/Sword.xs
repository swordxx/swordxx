
#ifdef __cplusplus
extern "C" {
#endif
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#ifdef __cplusplus
}
#endif

#undef list

#include <iostream.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <localemgr.h>

SWMgr manager;

class VerseIterator {
public:
	SWModule * module;
	VerseKey key;
	VerseIterator(char * module_name);
	~VerseIterator();

	// Iterator info
	const char * get_module_name() { return module->Name(); }

	// Verse info
	const char * get_verse() { module->SetKey(key); return (const char*)*module; }
	const char * get_key() { return key; }

	// Iteration
	int next(int how_far);
	int prev(int how_far);

	// Go to verse
	int go_to_verse(char * verse);

	// Book / Chapter / Verse traversal
	const char * get_book() { return key.books[key.Testament() - 1][key.Book() - 1].name; }
	int get_chapter_num() { return key.Chapter(); }
	int get_verse_num() { return key.Verse(); }

	void set_chapter_num(int chapter_num) { key.Chapter(chapter_num); }
	void set_verse_num(int verse_num) { key.Verse(verse_num); }

	// Stuff that should go into some sort of Module class
	char * _get_module_name() { return module->Name(); }
	char * _get_module_desc() { return module->Description(); }
	char * _get_module_type() { return module->Type(); }
	int _verse_greater(char * verse1, char * verse2);
};

VerseIterator::VerseIterator(char * module_name) {
	module = manager.Modules[module_name];
	key = module->Key();
	key.setLocale(LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());

	key = key.LowerBound();
}

VerseIterator::~VerseIterator() {
}

int VerseIterator::next(int how_far) {
	if(key == key.UpperBound()) {
		return 0;
	}
	module->SetKey(key);
	(*module) += how_far;
	key = module->Key();
	return 1;
}

int VerseIterator::prev(int how_far) {
	if(key == key.LowerBound()) {
		return 0;
	}
	module->SetKey(key);
	(*module) -= how_far;
	key = module->Key();
	return 1;
}

int VerseIterator::go_to_verse(char * verse) {
	int retval = module->SetKey(verse);
	key = module->Key();
	return ! retval;
}

int VerseIterator::_verse_greater(char * verse1, char * verse2) {
	if(module->SetKey(verse1)) {
		return -1;
	}
	SWKey key1 = module->Key();
	if(module->SetKey(verse2)) {
		return -1;
	}
	SWKey key2 = module->Key();
	return key1 > key2;
}

MODULE = Sword		PACKAGE = Sword		

void
get_modules(char * textType = "Biblical Texts")
PPCODE:
	ModMap::iterator modIterator;
	for (modIterator = manager.Modules.begin(); modIterator != manager.Modules.end(); modIterator++) {
		SWModule *module = (*modIterator).second;
		if(!strcmp(module->Type(),textType)) {
			string s = (*modIterator).first;
			XPUSHs(sv_2mortal(newSVpv(s.c_str(), s.length())));
		}
	}

void
get_locales()
PPCODE:
	LocaleMgr *lm = LocaleMgr::getSystemLocaleMgr();
	list<SWBuf> loclist = lm->getAvailableLocales();
	list<SWBuf>::iterator li = loclist.begin();
	for (;li != loclist.end(); li++) {
		XPUSHs(sv_2mortal(newSVpv(li->c_str(), li->length())));
	}

void
set_locale(char * locale)
CODE:
	LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(locale);


MODULE = Sword		PACKAGE = Sword::VerseIterator

VerseIterator *
VerseIterator::new(char * module_name)

void
VerseIterator::DESTROY()

const char *
VerseIterator::get_module_name()

const char *
VerseIterator::get_verse()

const char *
VerseIterator::get_key()


int
VerseIterator::next(int how_far = 1)

int
VerseIterator::prev(int how_far = 1)


int
VerseIterator::go_to_verse(char * verse)


const char *
VerseIterator::get_book()

int
VerseIterator::get_chapter_num()

int
VerseIterator::get_verse_num()

void
VerseIterator::set_chapter_num(int chapter_num)

void
VerseIterator::set_verse_num(int verse_num)


void
VerseIterator::_get_books()
PPCODE:

	for(int testament_num=0;testament_num<2;testament_num++) {
		for(int book_num=0;book_num<THIS->key.BMAX[testament_num];book_num++) {
			int num_chapters = THIS->key.books[testament_num][book_num].chapmax;
			HV * book_info = newHV();
			hv_store(book_info, "name", 4, newSVpv(THIS->key.books[testament_num][book_num].name, 0), 0);
			hv_store(book_info, "num_chapters", 12, newSVnv(num_chapters), 0);
			hv_store(book_info, "testament_num", 13, newSVnv(testament_num), 0);
			hv_store(book_info, "book_num", 8, newSVnv(book_num), 0);
			AV * chapter_maxes = newAV();
			for(int i=0;i<num_chapters;i++) {
				av_push(chapter_maxes, newSVnv(THIS->key.books[testament_num][book_num].versemax[i]));
			}
			hv_store(book_info, "chapter_maxes", 13, newRV_noinc((SV *)chapter_maxes), 0);
			XPUSHs(newRV_noinc((SV *)book_info));
		}
	}

void
VerseIterator::_search(char * searchStr)
PPCODE:
	ListKey &searchResults = THIS->module->Search(searchStr);

	searchResults.Persist(true);

	for(int i=0;i<searchResults.Count();i++) {
		SWKey * key = searchResults.GetElement(i);
		AV * search_result = newAV();
		av_push(search_result, newSVpv(*key, 0));
		THIS->module->SetKey(key);
		av_push(search_result, newSVpv(*THIS->module, 0));
		XPUSHs(newRV_noinc((SV *)search_result));
	}

void
VerseIterator::_search_verses(char * searchStr)
PPCODE:
	ListKey &searchResults = THIS->module->Search(searchStr);

	searchResults.Persist(true);

	for(int i=0;i<searchResults.Count();i++) {
		SWKey * key = searchResults.GetElement(i);
		XPUSHs(newSVpv(*key, 0));
	}

void
VerseIterator::_parse_verse_list(char * verseList)
PPCODE:
	ListKey searchResults = THIS->key.ParseVerseList(verseList, "Genesis 1:1", false);

	searchResults.Persist(true);

	for(int i=0;i<searchResults.Count();i++) {
		SWKey * key = searchResults.GetElement(i);
		XPUSHs(newSVpv(*key, 0));
	}

char *
VerseIterator::_get_module_name()

char *
VerseIterator::_get_module_desc()

char *
VerseIterator::_get_module_type()

int
VerseIterator::_verse_greater(char * verse1, char * verse2)

