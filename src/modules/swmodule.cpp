/******************************************************************************
 *  swmodule.cpp -code for base class 'module'.  Module is the basis for all
 *		  types of modules (e.g. texts, commentaries, maps, lexicons,
 *		  etc.)
 */

#include <sysdata.h>
#include <swmodule.h>
#include <utilstr.h>
#include <regex.h>	// GNU
#include <swfilter.h>
#include <versekey.h>	// KLUDGE for Search
#include <treekeyidx.h>	// KLUDGE for Search
#include <swoptfilter.h>
#include <filemgr.h>
#ifndef _MSC_VER
#include <iostream>
#endif

#ifdef USELUCENE
#include <CLucene.h>
//Lucence includes
#include "CLucene.h"
#include "CLucene/util/Reader.h"
#include "CLucene/util/Misc.h"
#include "CLucene/util/dirent.h"

using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::util;
using namespace lucene::store;
using namespace lucene::document;
using namespace lucene::queryParser;
using namespace lucene::search;
#endif


SWORD_NAMESPACE_START

SWDisplay SWModule::rawdisp;

typedef std::list<SWBuf> StringList;

/******************************************************************************
 * SWModule Constructor - Initializes data for instance of SWModule
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 *	imodtype - Type of Module (All modules will be displayed with
 *			others of same type under their modtype heading
 *	unicode  - if this module is unicode
 */

SWModule::SWModule(const char *imodname, const char *imoddesc, SWDisplay *idisp, char *imodtype, SWTextEncoding encoding, SWTextDirection direction, SWTextMarkup markup, const char* imodlang) {
	key       = CreateKey();
	entryBuf  = "";
	config    = &ownConfig;
	modname   = 0;
	error     = 0;
	moddesc   = 0;
	modtype   = 0;
	modlang   = 0;
	this->encoding = encoding;
	this->direction = direction;
	this->markup  = markup;
	entrySize= -1;
	disp     = (idisp) ? idisp : &rawdisp;
	stdstr(&modname, imodname);
	stdstr(&moddesc, imoddesc);
	stdstr(&modtype, imodtype);
	stdstr(&modlang, imodlang);
	stripFilters = new FilterList();
	rawFilters = new FilterList();
	renderFilters = new FilterList();
	optionFilters = new OptionFilterList();
	encodingFilters = new FilterList();
	skipConsecutiveLinks = true;
	procEntAttr = true;
}


/******************************************************************************
 * SWModule Destructor - Cleans up instance of SWModule
 */

SWModule::~SWModule()
{
	if (modname)
		delete [] modname;
	if (moddesc)
		delete [] moddesc;
	if (modtype)
		delete [] modtype;
	if (modlang)
		delete [] modlang;

	if (key) {
		if (!key->Persist())
			delete key;
	}

	stripFilters->clear();
     rawFilters->clear();
     renderFilters->clear();
     optionFilters->clear();
     encodingFilters->clear();
	entryAttributes.clear();

     delete stripFilters;
     delete rawFilters;
     delete renderFilters;
     delete optionFilters;
     delete encodingFilters;
}


/******************************************************************************
 * SWModule::CreateKey - Allocates a key of specific type for module
 *
 * RET:	pointer to allocated key
 */

SWKey *SWModule::CreateKey()
{
	return new SWKey();
}


/******************************************************************************
 * SWModule::Error - Gets and clears error status
 *
 * RET:	error status
 */

char SWModule::Error()
{
	char retval = error;

	error = 0;
	return retval;
}


/******************************************************************************
 * SWModule::Name - Sets/gets module name
 *
 * ENT:	imodname - value which to set modname
 *		[0] - only get
 *
 * RET:	pointer to modname
 */

char *SWModule::Name(const char *imodname) {
	return stdstr(&modname, imodname);
}

char *SWModule::Name() const {
	return modname;
}


/******************************************************************************
 * SWModule::Description - Sets/gets module description
 *
 * ENT:	imoddesc - value which to set moddesc
 *		[0] - only get
 *
 * RET:	pointer to moddesc
 */

char *SWModule::Description(const char *imoddesc) {
	return stdstr(&moddesc, imoddesc);
}

char *SWModule::Description() const {
	return moddesc;
}


/******************************************************************************
 * SWModule::Type - Sets/gets module type
 *
 * ENT:	imodtype - value which to set modtype
 *		[0] - only get
 *
 * RET:	pointer to modtype
 */

char *SWModule::Type(const char *imodtype) {
	return stdstr(&modtype, imodtype);
}

char *SWModule::Type() const {
	return modtype;
}

/******************************************************************************
 * SWModule::Direction - Sets/gets module direction
 *
 * ENT:	newdir - value which to set direction
 *		[-1] - only get
 *
 * RET:	char direction
 */
char SWModule::Direction(signed char newdir) {
        if (newdir != -1)
                direction = newdir;
        return direction;
}

/******************************************************************************
 * SWModule::Encoding - Sets/gets module encoding
 *
 * ENT:	newdir - value which to set direction
 *		[-1] - only get
 *
 * RET:	char encoding
 */
char SWModule::Encoding(signed char newenc) {
        if (newenc != -1)
                encoding = newenc;
        return encoding;
}

/******************************************************************************
 * SWModule::Markup - Sets/gets module markup
 *
 * ENT:	newdir - value which to set direction
 *		[-1] - only get
 *
 * RET:	char markup
 */
char SWModule::Markup(signed char newmark) {
        if (newmark != -1)
                markup = newmark;
        return markup;
}


/******************************************************************************
 * SWModule::Lang - Sets/gets module language
 *
 * ENT:	imodlang - value which to set modlang
 *		[0] - only get
 *
 * RET:	pointer to modname
 */

char *SWModule::Lang(const char *imodlang)
{
	return stdstr(&modlang, imodlang);
}


/******************************************************************************
 * SWModule::Disp - Sets/gets display driver
 *
 * ENT:	idisp - value which to set disp
 *		[0] - only get
 *
 * RET:	pointer to disp
 */

SWDisplay *SWModule::getDisplay() const {
	return disp;
}

void SWModule::setDisplay(SWDisplay *idisp) {
	disp = idisp;
}


/******************************************************************************
 * SWModule::Display - Calls this modules display object and passes itself
 *
 * RET:	error status
 */

char SWModule::Display() {
	disp->Display(*this);
	return 0;
}


/******************************************************************************
 * SWModule::getKey - Gets the key from this module that points to the position
 *			record
 *
 * RET:	key object
 */

SWKey *SWModule::getKey() const {
	return key;
}


/******************************************************************************
 * SWModule::setKey - Sets a key to this module for position to a particular
 *			record
 *
 * ENT:	ikey - key with which to set this module
 *
 * RET:	error status
 */

char SWModule::setKey(const SWKey *ikey) {
	SWKey *oldKey = 0;

	if (key) {
		if (!key->Persist())	// if we have our own copy
			oldKey = key;
	}

	if (!ikey->Persist()) {		// if we are to keep our own copy
		 key = CreateKey();
		*key = *ikey;
	}
	else	 key = (SWKey *)ikey;		// if we are to just point to an external key

	if (oldKey)
		delete oldKey;

	return 0;
}


/******************************************************************************
 * SWModule::setPosition(SW_POSITION)	- Positions this modules to an entry
 *
 * ENT:	p	- position (e.g. TOP, BOTTOM)
 *
 * RET: *this
 */

void SWModule::setPosition(SW_POSITION p) {
	*key = p;
	char saveError = key->Error();

	switch (p) {
	case POS_TOP:
		(*this)++;
		(*this)--;
		break;

	case POS_BOTTOM:
		(*this)--;
		(*this)++;
		break;
	}

	error = saveError;
}


/******************************************************************************
 * SWModule::increment	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

void SWModule::increment(int steps) {
	(*key) += steps;
	error = key->Error();
}


/******************************************************************************
 * SWModule::decrement	- Decrements module key a number of entries
 *
 * ENT:	decrement	- Number of entries to jump backward
 *
 * RET: *this
 */

void SWModule::decrement(int steps) {
	(*key) -= steps;
	error = key->Error();
}


/******************************************************************************
 * SWModule::Search 	- Searches a module for a string
 *
 * ENT:	istr		- string for which to search
 * 	searchType	- type of search to perform
 *				>=0 - regex
 *				-1  - phrase
 *				-2  - multiword
 *				-3  - entryAttrib (eg. Word//Lemma/G1234/)
 *				-4  - clucene
 * 	flags		- options flags for search
 *	justCheckIfSupported	- if set, don't search, only tell if this
 *							function supports requested search.
 *
 * RET: listkey set to verses that contain istr
 */

ListKey &SWModule::search(const char *istr, int searchType, int flags, SWKey *scope, bool *justCheckIfSupported, void (*percent)(char, void *), void *percentUserData) {

	listkey.ClearList();

#ifdef USELUCENE
	SWBuf target = getConfigEntry("AbsoluteDataPath");
	char ch = target.c_str()[strlen(target.c_str())-1];
	if ((ch != '/') && (ch != '\\'))
		target.append('/');
	target.append("lucene");
#endif
	if (justCheckIfSupported) {
		*justCheckIfSupported = (searchType >= -3);
#ifdef USELUCENE
		if ((searchType == -4) && (IndexReader::indexExists(target.c_str()))) {
			*justCheckIfSupported = true;
		}
#endif
		return listkey;
	}
	
	SWKey *savekey = 0;
	SWKey *searchkey = 0;
	regex_t preg;
	SWKey textkey;
	char **words = 0;
	char *wordBuf = 0;
	int wordCount = 0;
	const char *sres;
	terminateSearch = false;
	char perc = 1;
	bool savePEA = isProcessEntryAttributes();

	processEntryAttributes(searchType == -3);
	

	if (!key->Persist()) {
		savekey = CreateKey();
		*savekey = *key;
	}
	else	savekey = key;

	searchkey = (scope)?scope->clone():(key->Persist())?key->clone():0;
	if (searchkey) {
		searchkey->Persist(1);
		setKey(*searchkey);
	}

	(*percent)(perc, percentUserData);
	// MAJOR KLUDGE: VerseKey::Index still return index within testament.
	// 	VerseKey::NewIndex should be moved to Index and Index should be some
	// 	VerseKey specific name
	VerseKey *vkcheck = 0;
	SWTRY {
		vkcheck = SWDYNAMIC_CAST(VerseKey, key);
	}
	SWCATCH (...) {}
	// end MAJOR KLUDGE

	*this = BOTTOM;
	// fix below when we find out the bug
	long highIndex = (vkcheck)?32300/*vkcheck->NewIndex()*/:key->Index();
	if (!highIndex)
		highIndex = 1;		// avoid division by zero errors.
	*this = TOP;
	if (searchType >= 0) {
		flags |=searchType|REG_NOSUB|REG_EXTENDED;
		regcomp(&preg, istr, flags);
	}

	(*percent)(++perc, percentUserData);


#ifdef USELUCENE
	if (searchType == -4) {	// lucene
		//Buffers for the wchar<->utf8 char* conversion
		enum {
			MAX_CONV_SIZE = 2047
		};
		wchar_t wcharBuffer[MAX_CONV_SIZE + 1];
		char utfBuffer[MAX_CONV_SIZE + 1];
		
		
		// Make sure our scope for this search is bounded by
		// something we can test
		// In the future, add bool SWKey::isValid(const char *tryString);
		VerseKey vk;
		bool freeTestKey = false;
		SWKey *testKey = 0;
		SWTRY {
			testKey = SWDYNAMIC_CAST(VerseKey, ((scope)?scope:key));
			if (!testKey) {
				testKey = SWDYNAMIC_CAST(ListKey, ((scope)?scope:key));
			}
		}
		SWCATCH ( ... ) {}
		if (!testKey) {
			testKey = new ListKey();
			*testKey = vk.ParseVerseList((const char *)((scope)?scope:key), vk, true);
			freeTestKey = true;
		}
		lucene::index::IndexReader *ir;
		lucene::search::IndexSearcher *is;
		ir = IndexReader::open(target);
		is = new IndexSearcher(ir);
		(*percent)(10, percentUserData);

		standard::StandardAnalyzer analyzer;
		lucene_utf8towcs(wcharBuffer, istr, MAX_CONV_SIZE); //TODO Is istr always utf8?
		Query *q =  QueryParser::parse(wcharBuffer, _T("content"), &analyzer);
		(*percent)(20, percentUserData);
		Hits *h = is->search(q);
		(*percent)(80, percentUserData);


		// iterate thru each good module position that meets the search
		for (long i = 0; i < h->length(); i++) {
			Document &doc = h->doc(i);

			// set a temporary verse key to this module position
			lucene_wcstoutf8(utfBuffer, doc.get(_T("key")), MAX_CONV_SIZE);	
		       	vk = utfBuffer; //TODO Does vk always accept utf8?

			// check scope
			// Try to set our scope key to this verse key
			*testKey = vk;

			// check to see if it set ok and if so, add to our return list
			if (*testKey == vk) {
				listkey << (const char *) vk;
				listkey.GetElement()->userData = (void *)((__u32)(h->score(i)*100));
			}
		}
		(*percent)(98, percentUserData);

		delete h;
		delete q;

		delete is;
		ir->close();
		if (freeTestKey) {
			delete testKey;
		}
	}
#endif


	// multi-word
	if (searchType == -2) {
		wordBuf = (char *)calloc(sizeof(char), strlen(istr) + 1);
		strcpy(wordBuf, istr);
		words = (char **)calloc(sizeof(char *), 10);
		int allocWords = 10;
		words[wordCount] = strtok(wordBuf, " ");
		while (words[wordCount]) {
			wordCount++;
			if (wordCount == allocWords) {
				allocWords+=10;
				words = (char **)realloc(words, sizeof(char *)*allocWords);
			}
			words[wordCount] = strtok(NULL, " ");
		}
	}

	// entry attributes
	if (searchType == -3) {
		wordBuf = (char *)calloc(sizeof(char), strlen(istr) + 1);
		char *checkSlash = wordBuf;
		strcpy(wordBuf, istr);
		words = (char **)calloc(sizeof(char *), 10);
		int allocWords = 10;
		while (*checkSlash == '/')
			words[wordCount++] = checkSlash++;
		words[wordCount] = strtok(wordBuf, "/");
		while (words[wordCount]) {
			wordCount++;
			if (wordCount == allocWords) {
				allocWords+=10;
				words = (char **)realloc(words, sizeof(char *)*allocWords);
			}
			checkSlash = words[wordCount-1] + (strlen(words[wordCount-1]))+1;
			while (*checkSlash == '/')
				words[wordCount++] = checkSlash++;
			words[wordCount] = strtok(NULL, "/");
		}
		for (int i = 0; i < wordCount; i++) {
			if (words[i][0] == '/')
				words[i][0] = 0;
		}
	}

	perc = 5;
	(*percent)(perc, percentUserData);

	
	while ((searchType > -4) && !Error() && !terminateSearch) {
		long mindex = 0;
		if (vkcheck)
			mindex = vkcheck->NewIndex();
		else mindex = key->Index();
		float per = (float)mindex / highIndex;
		per *= 93;
		per += 5;
		char newperc = (char)per;
//		char newperc = (char)(5+(93*(((float)((vkcheck)?vkcheck->NewIndex():key->Index()))/highIndex)));
		if (newperc > perc) {
			perc = newperc;
			(*percent)(perc, percentUserData);
		}
		else if (newperc < perc) {
#ifndef _MSC_VER
			std::cerr << "Serious error: new percentage complete is less than previous value\n";
			std::cerr << "using vk? " << ((vkcheck)?"yes":"no") << "\n";
			std::cerr << "index: " << ((vkcheck)?vkcheck->NewIndex():key->Index()) << "\n";
			std::cerr << "highIndex: " << highIndex << "\n";
			std::cerr << "newperc ==" << (int)newperc << "%" << "is smaller than\n";
			std::cerr << "perc == "  << (int )perc << "% \n";
#endif
		}
		if (searchType >= 0) {
			if (!regexec(&preg,  StripText(), 0, 0, 0)) {
				textkey = KeyText();
				listkey << textkey;
			}
		}

		// phrase
		else if (searchType == -1) {
			sres = ((flags & REG_ICASE) == REG_ICASE) ? stristr(StripText(), istr) : strstr(StripText(), istr);
			if (sres) { //it's also in the StripText(), so we have a valid search result item now
				textkey = KeyText();
				listkey << textkey;
			}
		}

		// multiword
		else if (searchType == -2) {
			int loopCount = 0;
			int foundWords = 0;
			bool specialStrips = getConfigEntry("LocalStripFilter");
			do {
				const char* textBuf = ((loopCount == 0)&&(!specialStrips)) ? getRawEntry() : StripText();
				foundWords = 0;
				
				for (int i = 0; i < wordCount; ++i) {
					sres = ((flags & REG_ICASE) == REG_ICASE) ? stristr(textBuf, words[i]) : strstr(textBuf, words[i]);
					if (!sres) {
						break; //for loop
					}
					++foundWords;
				}
				
				++loopCount;
			} while ( (loopCount < 2) && (foundWords == wordCount));
			
			if ((loopCount == 2) && (foundWords == wordCount)) { //we found the right words in both raw and stripped text, which means it's a valid result item
				textkey = KeyText();
				listkey << textkey;
			}
		}

		// entry attributes
		else if (searchType == -3) {
			int i;
			RenderText();	// force parse
			AttributeTypeList &entryAttribs = getEntryAttributes();
			AttributeTypeList::iterator i1Start, i1End;
			AttributeList::iterator i2Start, i2End;
			AttributeValue::iterator i3Start, i3End;

			if ((words[0]) && (words[0][0])) {
				i1Start = entryAttribs.find(words[0]);
				i1End = i1Start;
				if (i1End != entryAttribs.end())
				i1End++;
			}
			else {
				i1Start = entryAttribs.begin();
				i1End   = entryAttribs.end();
			}
			for (;i1Start != i1End; i1Start++) {
				if ((words[1]) && (words[1][0])) {
					i2Start = i1Start->second.find(words[1]);
					i2End = i2Start;
					if (i2End != i1Start->second.end())
						i2End++;
				}
				else {
					i2Start = i1Start->second.begin();
					i2End   = i1Start->second.end();
				}
				for (;i2Start != i2End; i2Start++) {
					if ((words[2]) && (words[2][0])) {
						i3Start = i2Start->second.find(words[2]);
						i3End = i3Start;
						if (i3End != i2Start->second.end())
							i3End++;
					}
					else {
						i3Start = i2Start->second.begin();
						i3End   = i2Start->second.end();
					}
					for (;i3Start != i3End; i3Start++) {
						if (flags & SEARCHFLAG_MATCHWHOLEENTRY) {
							bool found = !(((flags & REG_ICASE) == REG_ICASE) ? stricmp(i3Start->second.c_str(), words[3]) : strcmp(i3Start->second.c_str(), words[3]));
							sres = (found) ? i3Start->second.c_str() : 0;
						}
						else {
							sres = ((flags & REG_ICASE) == REG_ICASE) ? stristr(i3Start->second.c_str(), words[3]) : strstr(i3Start->second.c_str(), words[3]);
						}
						if (sres) {
							textkey = KeyText();
							listkey << textkey;
							break;
						}
					}
					if (i3Start != i3End)
						break;
				}
				if (i2Start != i2End)
					break;
			}
		}
		(*this)++;
	}
	
	if (searchType >= 0)
		regfree(&preg);

	if (searchType == -2) {
		free(words);
		free(wordBuf);
	}

	setKey(*savekey);

	if (!savekey->Persist())
		delete savekey;

	if (searchkey)
		delete searchkey;

	listkey = TOP;
	processEntryAttributes(savePEA);
	(*percent)(100, percentUserData);

	return listkey;
}


/******************************************************************************
 * SWModule::StripText() 	- calls all stripfilters on current text
 *
 * ENT:	buf	- buf to massage instead of this modules current text
 * 	len	- max len of buf
 *
 * RET: this module's text at specified key location massaged by Strip filters
 */

const char *SWModule::StripText(const char *buf, int len) {
	return RenderText(buf, len, false);
}


/******************************************************************************
 * SWModule::RenderText 	- calls all renderfilters on current text
 *
 * ENT:	buf	- buffer to Render instead of current module position
 *
 * RET: listkey set to verses that contain istr
 */

 const char *SWModule::RenderText(const char *buf, int len, bool render) {
	entryAttributes.clear();

	static SWBuf local;
	if (buf)
		local = buf;

	SWBuf &tmpbuf = (buf) ? local : getRawEntryBuf();
	SWKey *key = 0;
	static char *null = "";

	if (tmpbuf) {
		unsigned long size = (len < 0) ? ((getEntrySize()<0) ? strlen(tmpbuf) : getEntrySize()) : len;
		if (size > 0) {
			key = (SWKey *)*this;

			optionFilter(tmpbuf, key);
	
			if (render) {
				renderFilter(tmpbuf, key);
				encodingFilter(tmpbuf, key);
			}
			else	stripFilter(tmpbuf, key);
		}
	}
	else {
		tmpbuf = null;
	}

	return tmpbuf;
}


/******************************************************************************
 * SWModule::RenderText 	- calls all renderfilters on current text
 *
 * ENT:	tmpKey	- key to use to grab text
 *
 * RET: this module's text at specified key location massaged by RenderFilers
 */

 const char *SWModule::RenderText(SWKey *tmpKey) {
	SWKey *savekey;
	const char *retVal;

	if (!key->Persist()) {
		savekey = CreateKey();
		*savekey = *key;
	}
	else	savekey = key;

	setKey(*tmpKey);

	retVal = RenderText();

	setKey(*savekey);

	if (!savekey->Persist())
		delete savekey;

	return retVal;
}


/******************************************************************************
 * SWModule::StripText 	- calls all StripTextFilters on current text
 *
 * ENT:	tmpKey	- key to use to grab text
 *
 * RET: this module's text at specified key location massaged by Strip filters
 */

const char *SWModule::StripText(SWKey *tmpKey) {
	SWKey *savekey;
	const char *retVal;

	if (!key->Persist()) {
		savekey = CreateKey();
		*savekey = *key;
	}
	else	savekey = key;

	setKey(*tmpKey);

	retVal = StripText();

	setKey(*savekey);

	if (!savekey->Persist())
		delete savekey;

	return retVal;
}


const char *SWModule::getConfigEntry(const char *key) const {
	ConfigEntMap::iterator it = config->find(key);
	return (it != config->end()) ? it->second.c_str() : 0;
}


void SWModule::setConfig(ConfigEntMap *config) {
	this->config = config;
}


bool SWModule::hasSearchFramework() {
#ifdef USELUCENE
	return true;
#else
	return SWSearchable::hasSearchFramework();
#endif
}

void SWModule::deleteSearchFramework() {
#ifdef USELUCENE
	SWBuf target = getConfigEntry("AbsoluteDataPath");
	char ch = target.c_str()[strlen(target.c_str())-1];
	if ((ch != '/') && (ch != '\\'))
		target.append('/');
	target.append("lucene");

	FileMgr::removeDir(target.c_str());
#else
	SWSearchable::deleteSearchFramework();
#endif
}


signed char SWModule::createSearchFramework(void (*percent)(char, void *), void *percentUserData) {
#ifdef USELUCENE
	SWKey *savekey = 0;
	SWKey *searchkey = 0;
	SWKey textkey;
	char *word = 0;
	char *wordBuf = 0;


	// turn all filters to default values
	StringList filterSettings;
	for (OptionFilterList::iterator filter = optionFilters->begin(); filter != optionFilters->end(); filter++) {
		filterSettings.push_back((*filter)->getOptionValue());
		(*filter)->setOptionValue(*((*filter)->getOptionValues().begin()));

		if (!strcmp("Greek Accents", (*filter)->getOptionName())) {
			(*filter)->setOptionValue("Off");
		}
	}

	
	// be sure we give CLucene enough file handles	
	FileMgr::getSystemFileMgr()->flush();

	// save key information so as not to disrupt original
	// module position
	if (!key->Persist()) {
		savekey = CreateKey();
		*savekey = *key;
	}
	else	savekey = key;

	searchkey = (key->Persist())?key->clone():0;
	if (searchkey) {
		searchkey->Persist(1);
		setKey(*searchkey);
	}

	IndexWriter *writer = NULL;
	Directory *d = NULL;
 
	standard::StandardAnalyzer *an = new standard::StandardAnalyzer();
	SWBuf target = getConfigEntry("AbsoluteDataPath");
	char ch = target.c_str()[strlen(target.c_str())-1];
	if ((ch != '/') && (ch != '\\'))
		target.append('/');
	target.append("lucene");
	FileMgr::createParent(target+"/dummy");

	if (IndexReader::indexExists(target.c_str())) {
		d = FSDirectory::getDirectory(target.c_str(), false);
		if (IndexReader::isLocked(d)) {
			IndexReader::unlock(d);
		}
																		   
		writer = new IndexWriter( d, an, false);
	} else {
		d = FSDirectory::getDirectory(target.c_str(), true);
		writer = new IndexWriter( d ,an, true);
	}


 
	char perc = 1;
	VerseKey *vkcheck = 0;
	vkcheck = SWDYNAMIC_CAST(VerseKey, key);

	TreeKeyIdx *tkcheck = 0;
	tkcheck = SWDYNAMIC_CAST(TreeKeyIdx, key);


	*this = BOTTOM;
	long highIndex = (vkcheck)?32300/*vkcheck->NewIndex()*/:key->Index();
	if (!highIndex)
		highIndex = 1;		// avoid division by zero errors.

	bool savePEA = isProcessEntryAttributes();
	processEntryAttributes(true);

	// prox chapter blocks
	// position module at the beginning
	*this = TOP;

	VerseKey chapMax;
	SWBuf proxBuf;
	SWBuf proxLem;
	SWBuf strong;

	const short int MAX_CONV_SIZE = 2047;
	wchar_t wcharBuffer[MAX_CONV_SIZE + 1];
	
	char err = Error();
	while (!err) {
		long mindex = 0;
		if (vkcheck)
			mindex = vkcheck->NewIndex();
		else mindex = key->Index();

		proxBuf = "";
		proxLem = "";

		// computer percent complete so we can report to our progress callback
		float per = (float)mindex / highIndex;
		// between 5%-98%
		per *= 93; per += 5;
		char newperc = (char)per;
		if (newperc > perc) {
			perc = newperc;
			(*percent)(perc, percentUserData);
		}

		// get "content" field
		const char *content = StripText();

		bool good = false;

		// start out entry
		Document *doc = new Document();
		// get "key" field
		SWBuf keyText = (vkcheck) ? vkcheck->getOSISRef() : getKeyText();
		if (content && *content) {
			good = true;

			
			// build "strong" field
			AttributeTypeList::iterator words;
			AttributeList::iterator word;
			AttributeValue::iterator strongVal;

			words = getEntryAttributes().find("Word");
			if (words != getEntryAttributes().end()) {
				for (word = words->second.begin();word != words->second.end(); word++) {
					strongVal = word->second.find("Lemma");
					if (strongVal != word->second.end()) {
						// cheeze.  skip empty article tags that weren't assigned to any text
						if (strongVal->second == "G3588") {
							if (word->second.find("Text") == word->second.end())
								continue;	// no text? let's skip
						}
						strong.append(strongVal->second);
						strong.append(' ');
					}
				}
			}

			lucene_utf8towcs(wcharBuffer, keyText, MAX_CONV_SIZE); //keyText must be utf8
			doc->add( *Field::UnIndexed(_T("key"), wcharBuffer ) );

			lucene_utf8towcs(wcharBuffer, content, MAX_CONV_SIZE); //content must be utf8
			doc->add( *Field::UnStored(_T("content"), wcharBuffer) );

			if (strong.length() > 0) {
				lucene_utf8towcs(wcharBuffer, strong, MAX_CONV_SIZE);
				doc->add( *Field::UnStored(_T("lemma"), wcharBuffer) );
//printf("setting fields (%s).\ncontent: %s\nlemma: %s\n", (const char *)*key, content, strong.c_str());
			}

			printf("setting fields (%s).\n", (const char *)*key);
			fflush(stdout);
		}
		// don't write yet, cuz we have to see if we're the first of a prox block (5:1 or chapter5/verse1

		// for VerseKeys use chapter
		if (vkcheck) {
			chapMax = *vkcheck;
			// we're the first verse in a chapter
			if (vkcheck->Verse() == 1) {
				chapMax = MAXVERSE;
				VerseKey saveKey = *vkcheck;
				while ((!err) && (*vkcheck <= chapMax)) {
//printf("building proxBuf from (%s).\nproxBuf.c_str(): %s\n", (const char *)*key, proxBuf.c_str());
printf("building proxBuf from (%s).\n", (const char *)*key);

					// build "strong" field
					strong = "";
					AttributeTypeList::iterator words;
					AttributeList::iterator word;
					AttributeValue::iterator strongVal;

					words = getEntryAttributes().find("Word");
					if (words != getEntryAttributes().end()) {
						for (word = words->second.begin();word != words->second.end(); word++) {
							strongVal = word->second.find("Lemma");
							if (strongVal != word->second.end()) {
								// cheeze.  skip empty article tags that weren't assigned to any text
								if (strongVal->second == "G3588") {
									if (word->second.find("Text") == word->second.end())
										continue;	// no text? let's skip
								}
								strong.append(strongVal->second);
								strong.append(' ');
							}
						}
					}
					content = getRawEntry();
					if (content && *content) {
						proxBuf += content;
						proxBuf.append(' ');
					}
					proxLem += strong;
					if (proxLem.length()) 
						proxLem.append("\n");
					(*this)++;
					err = Error();
				}
				err = 0;
				*vkcheck = saveKey;
			}
		}
		
		// for TreeKeys use siblings if we have no children
		else if (tkcheck) {
			if (!tkcheck->hasChildren()) {
				if (!tkcheck->previousSibling()) {
					do {
printf("building proxBuf from (%s).\n", (const char *)*key);
fflush(stdout);

						// build "strong" field
						strong = "";
						AttributeTypeList::iterator words;
						AttributeList::iterator word;
						AttributeValue::iterator strongVal;

						words = getEntryAttributes().find("Word");
						if (words != getEntryAttributes().end()) {
							for (word = words->second.begin();word != words->second.end(); word++) {
								strongVal = word->second.find("Lemma");
								if (strongVal != word->second.end()) {
									// cheeze.  skip empty article tags that weren't assigned to any text
									if (strongVal->second == "G3588") {
										if (word->second.find("Text") == word->second.end())
											continue;	// no text? let's skip
									}
									strong.append(strongVal->second);
									strong.append(' ');
								}
							}
						}

						content = getRawEntry();
						if (content && *content) {
							proxBuf += content;
							proxBuf.append("\n");
						}
						proxLem += strong;
						if (proxLem.length()) 
							proxLem.append(' ');
					} while (tkcheck->nextSibling());
					tkcheck->parent();
					tkcheck->firstChild();
				}
				else tkcheck->nextSibling();	// reposition from our previousSibling test
			}
		}
		
		if (proxBuf.length() > 0) {
			printf("proxBuf before (%s).\n%s\n", (const char *)*key, proxBuf.c_str());
			proxBuf = StripText(proxBuf);
	
			lucene_utf8towcs(wcharBuffer, proxBuf, MAX_CONV_SIZE); //keyText must be utf8
		
			printf("proxBuf after (%s).\n%s\n", (const char *)*key, proxBuf.c_str());
			

			doc->add( *Field::UnStored(_T("prox"), wcharBuffer) );
			good = true;
		}
		if (proxLem.length() > 0) {
			lucene_utf8towcs(wcharBuffer, proxLem, MAX_CONV_SIZE); //keyText must be utf8
			doc->add( *Field::UnStored(_T("proxlem"), wcharBuffer) );
			good = true;
		}
		if (good) {
printf("writing (%s).\n", (const char *)*key);
fflush(stdout);
			writer->addDocument(doc);
		}
		delete doc;

		(*this)++;
		err = Error();
	}

	writer->optimize();
	writer->close();
	delete writer;
	delete an;

	// reposition module back to where it was before we were called
	setKey(*savekey);

	if (!savekey->Persist())
		delete savekey;

	if (searchkey)
		delete searchkey;

	processEntryAttributes(savePEA);

	// reset option filters back to original values
	StringList::iterator origVal = filterSettings.begin();
	for (OptionFilterList::iterator filter = optionFilters->begin(); filter != optionFilters->end(); filter++) {
		(*filter)->setOptionValue(*origVal++);
	}

	return 0;
#else
	return SWSearchable::createSearchFramework(percent, percentUserData);
#endif
}

/** OptionFilterBuffer a text buffer
 * @param filters the FilterList of filters to iterate
 * @param buf the buffer to filter
 * @param key key location from where this buffer was extracted
 */
void SWModule::filterBuffer(OptionFilterList *filters, SWBuf &buf, SWKey *key) {
	OptionFilterList::iterator it;
	for (it = filters->begin(); it != filters->end(); it++) {
		(*it)->processText(buf, key, this);
	}
}

/** FilterBuffer a text buffer
 * @param filters the FilterList of filters to iterate
 * @param buf the buffer to filter
 * @param key key location from where this buffer was extracted
 */
void SWModule::filterBuffer(FilterList *filters, SWBuf &buf, SWKey *key) {
	FilterList::iterator it;
	for (it = filters->begin(); it != filters->end(); it++) {
		(*it)->processText(buf, key, this);
	}
}


SWORD_NAMESPACE_END
