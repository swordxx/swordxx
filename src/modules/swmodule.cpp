/******************************************************************************
 *  swmodule.cpp -code for base class 'module'.  Module is the basis for all
 *		  types of modules (e.g. texts, commentaries, maps, lexicons,
 *		  etc.)
 */

#include <string.h>
#include <swmodule.h>
#include <utilfuns.h>
#include <regex.h>	// GNU
#include <swfilter.h>


SWDisplay SWModule::rawdisp;

/******************************************************************************
 * SWModule Constructor - Initializes data for instance of SWModule
 *
 * ENT:	imodname - Internal name for module
 *	imoddesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 *	imodtype - Type of Module (All modules will be displayed with
 *			others of same type under their modtype heading
 */

SWModule::SWModule(const char *imodname, const char *imoddesc, SWDisplay *idisp, char *imodtype)
{
	key      = CreateKey();
	entrybuf = new char [1];
	*entrybuf = 0;
	modname  = 0;
	error    = 0;
	moddesc  = 0;
	modtype  = 0;
	disp     = (idisp) ? idisp : &rawdisp;
	stdstr(&modname, imodname);
	stdstr(&moddesc, imoddesc);
	stdstr(&modtype, imodtype);
	render = true;	// for protected method when sometimes need RenderText not to _Render Text_ :)  kludge / rewrite
}


/******************************************************************************
 * SWModule Destructor - Cleans up instance of SWModule
 */

SWModule::~SWModule()
{
	if (entrybuf)
		delete [] entrybuf;
	if (modname)
		delete [] modname;
	if (moddesc)
		delete [] moddesc;
	if (modtype)
		delete [] modtype;
	if (key) {
		if (!key->Persist())
			delete key;
	}
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

char *SWModule::Name(char *imodname)
{
	return stdstr(&modname, imodname);
}


/******************************************************************************
 * SWModule::Description - Sets/gets module description
 *
 * ENT:	imoddesc - value which to set moddesc
 *		[0] - only get
 *
 * RET:	pointer to moddesc
 */

char *SWModule::Description(char *imoddesc)
{
	return stdstr(&moddesc, imoddesc);
}


/******************************************************************************
 * SWModule::Type - Sets/gets module type
 *
 * ENT:	imodtype - value which to set modtype
 *		[0] - only get
 *
 * RET:	pointer to modtype
 */

char *SWModule::Type(char *imodtype)
{
	return stdstr(&modtype, imodtype);
}


/******************************************************************************
 * SWModule::Disp - Sets/gets display driver
 *
 * ENT:	idisp - value which to set disp
 *		[0] - only get
 *
 * RET:	pointer to disp
 */

SWDisplay *SWModule::Disp(SWDisplay *idisp)
{
	if (idisp)
		disp = idisp;

	return disp;
}


/******************************************************************************
 * SWModule::Display - Calls this modules display object and passes itself
 *
 * RET:	error status
 */

char SWModule::Display()
{
	disp->Display(*this);
	return 0;
}


/******************************************************************************
 * SWModule::SetKey - Sets a key to this module for position to a particular
 *			record or set of records
 *
 * ENT:	ikey - key with which to set this module
 *
 * RET:	error status
 */

char SWModule::SetKey(const SWKey &ikey)
{
	if (key) {
		if (!key->Persist())	// if we have our own copy
			delete key;
	}

	if (!ikey.Persist()) {		// if we are to keep our own copy
		 key = CreateKey();
		*key = ikey;
	}
	else	 key = (SWKey *)&ikey;		// if we are to just point to an external key

	return 0;
}


/******************************************************************************
 * SWModule::KeyText - Sets/gets module KeyText
 *
 * ENT:	ikeytext - value which to set keytext
 *		[0] - only get
 *
 * RET:	pointer to keytext
 */

const char *SWModule::KeyText(char *ikeytext)
{
	if (ikeytext)
		SetKey((const char *)ikeytext);

	return *key;
}


/******************************************************************************
 * SWModule::operator =(POSITION)	- Positions this modules to an entry
 *
 * ENT:	p	- position (e.g. TOP, BOTTOM)
 *
 * RET: *this
 */

SWModule &SWModule::operator =(POSITION p)
{
	*key = p;
	error = key->Error();

	return *this;
}


/******************************************************************************
 * SWModule::operator +=	- Increments module key a number of entries
 *
 * ENT:	increment	- Number of entries to jump forward
 *
 * RET: *this
 */

SWModule &SWModule::operator +=(int increment)
{
	(*key) += increment;
	error = key->Error();

	return *this;
}


/******************************************************************************
 * SWModule::operator -=	- Decrements module key a number of entries
 *
 * ENT:	decrement	- Number of entries to jump backward
 *
 * RET: *this
 */

SWModule &SWModule::operator -=(int increment)
{
	(*key) -= increment;
	error = key->Error();

	return *this;
}


/******************************************************************************
 * SWModule::Search 	- Searches a module for a string
 *
 * ENT:	istr		- string for which to search
 * 	searchType	- type of search to perform
 *				>=0 - regex
 *				-1  - phrase
 *				-2  - multiword
 * 	flags		- options flags for search
 *
 * RET: listkey set to verses that contain istr
 */

ListKey &SWModule::Search(const char *istr, int searchType, int flags, SWKey *scope)
{
	SWKey *savekey = 0;
	SWKey *searchkey = 0;
	regex_t preg;
	SWKey textkey;
	char **words = 0;
	char *wordBuf = 0;
	int wordCount = 0;
	const char *sres;
	terminateSearch = false;

	listkey.ClearList();

	if (!key->Persist()) {
		savekey = CreateKey();
		*savekey = *key;
	}
	else	savekey = key;

	searchkey = (scope)?scope->clone():(key->Persist())?key->clone():0;
	if (searchkey) {
		searchkey->Persist(1);
		SetKey(*searchkey);
	}

	*this = TOP;
	if (searchType >= 0) {
		flags |=searchType|REG_NOSUB|REG_EXTENDED;
		regcomp(&preg, istr, flags);
	}

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




	while (!Error() && !terminateSearch) {
		if (searchType >= 0) {
			if (!regexec(&preg,  StripText(), 0, 0, 0)) {
				textkey = KeyText();
				listkey << textkey;
			}
		}
		else {
			if (searchType == -1) {
				sres = ((flags & REG_ICASE) == REG_ICASE) ? stristr(StripText(), istr) : strstr(StripText(), istr);
				if (sres) {
						textkey = KeyText();
						listkey << textkey;
				}
			}
			if (searchType == -2) {
				int i;
				for (i = 0; i < wordCount; i++) {
					sres = ((flags & REG_ICASE) == REG_ICASE) ? stristr(StripText(), words[i]) : strstr(StripText(), words[i]);
					if (!sres)
						break;
				}
				if (i == wordCount) {
					textkey = KeyText();
					listkey << textkey;
				}

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

	SetKey(*savekey);

	if (!savekey->Persist())
		delete savekey;

	if (searchkey)
		delete searchkey;

	listkey = TOP;
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

const char *SWModule::StripText(char *buf, int len)
{
	FilterList::iterator it;
	SWKey *key = 0;

	if (!buf) {
		key = (SWKey *)*this;
		render = false;
		buf = (char *)*this;
		render = true;
	}

	for (it = optionfilters.begin(); it != optionfilters.end(); it++) {
		(*it)->ProcessText(buf, len, key);
	}
	for (it = stripfilters.begin(); it != stripfilters.end(); it++) {
		(*it)->ProcessText(buf, len, key);
	}

	return buf;
}


/******************************************************************************
 * SWModule::RenderText 	- calls all renderfilters on current text
 *
 * ENT:	buf	- buffer to Render instead of current module position
 *
 * RET: listkey set to verses that contain istr
 */

 const char *SWModule::RenderText(char *buf, int len)
{
	FilterList::iterator it;
	SWKey *key = 0;

	if (!buf) {
		key = (SWKey *)*this;
		buf = (char *)*this;
	}

	for (it = optionfilters.begin(); it != optionfilters.end(); it++) {
		(*it)->ProcessText(buf, len, key);
	}
	
	if (render) {
		for (it = renderfilters.begin(); it != renderfilters.end(); it++) {
			(*it)->ProcessText(buf, len, key);
		}
	}

	return buf;
}


/******************************************************************************
 * SWModule::RenderText 	- calls all renderfilters on current text
 *
 * ENT:	tmpKey	- key to use to grab text
 *
 * RET: this module's text at specified key location massaged by RenderFilers
 */

 const char *SWModule::RenderText(SWKey *tmpKey)
{
	SWKey *savekey;
	const char *retVal;

	if (!key->Persist()) {
		savekey = CreateKey();
		*savekey = *key;
	}
	else	savekey = key;

	SetKey(*tmpKey);

	retVal = RenderText();

	SetKey(*savekey);

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

 const char *SWModule::StripText(SWKey *tmpKey)
{
	SWKey *savekey;
	const char *retVal;

	if (!key->Persist()) {
		savekey = CreateKey();
		*savekey = *key;
	}
	else	savekey = key;

	SetKey(*tmpKey);

	retVal = StripText();

	SetKey(*savekey);

	if (!savekey->Persist())
		delete savekey;

	return retVal;
}
