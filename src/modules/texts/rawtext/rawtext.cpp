/******************************************************************************
 *  rawtext.cpp - code for class 'RawText'- a module that reads raw text files:
 *		  ot and nt using indexs ??.bks ??.cps ??.vss
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <utilfuns.h>
#include <rawverse.h>
#include <rawtext.h>

#include <map>
#include <list>
#include <algorithm>
#include <regex.h>	// GNU

#ifndef O_BINARY
#define O_BINARY 0
#endif

/******************************************************************************
 * RawText Constructor - Initializes data for instance of RawText
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawText::RawText(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, bool unicode)
		: SWText(iname, idesc, idisp, unicode),
          RawVerse(ipath) {
          
	versebuf = 0;
	string fname;
	fname = path;
	char ch = fname.c_str()[strlen(fname.c_str())-1];
	if ((ch != '/') && (ch != '\\'))
		fname += "/";
	
	for (int loop = 0; loop < 2; loop++) {
     	fastSearch[loop] = 0;
		string fastidxname =(fname + ((loop)?"ntwords.dat":"otwords.dat"));
		if (!access(fastidxname.c_str(), 04)) {
			fastidxname = (fname + ((loop)?"ntwords.idx":"otwords.idx"));
			if (!access(fastidxname.c_str(), 04))
				fastSearch[loop] = new RawStr((fname + ((loop)?"ntwords":"otwords")).c_str());
        	}
	}
}


/******************************************************************************
 * RawText Destructor - Cleans up instance of RawText
 */

RawText::~RawText()
{
	if (versebuf)
		delete [] versebuf;

	if (fastSearch[0])
		delete fastSearch[0];

	if (fastSearch[1])
		delete fastSearch[1];
}


/******************************************************************************
 * RawText::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

char *RawText::getRawEntry() {
	long  start = 0;
	unsigned short size = 0;
	VerseKey *key = 0;

	// see if we have a VerseKey * or decendant
#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {	}
#endif
	// if we don't have a VerseKey * decendant, create our own
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);
	entrySize = size;        // support getEntrySize call
	if (versebuf)
		delete [] versebuf;

	versebuf = new char [ (size + 2) * FILTERPAD * ((unicode) ? 9 : 1 ) ];
	*versebuf = 0;

	gettext(key->Testament(), start, (size + 2), versebuf);

	rawFilter(versebuf, size, key);

     if (!unicode)
		preptext(versebuf);

	if (this->key != key) // free our key if we created a VerseKey
		delete key;

	return versebuf;
}


char RawText::createSearchFramework() {
	SWKey *savekey = 0;
	SWKey *searchkey = 0;
	SWKey textkey;
	char *word = 0;
	char *wordBuf = 0;

	// dictionary holds words associated with a list
	// containing every module position that contains
	// the word.  [0] Old Testament; [1] NT
	map < string, list<long> > dictionary[2];


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
		SetKey(*searchkey);
	}

	// position module at the beginning
	*this = TOP;

	VerseKey *lkey = (VerseKey *)key;

	// iterate thru each entry in module
	while (!Error()) {
		long index = lkey->Index();
		wordBuf = (char *)calloc(sizeof(char), strlen(StripText()) + 1);
		strcpy(wordBuf, StripText());

		// grab each word from the text
		word = strtok(wordBuf, " !.,?;:()-=+/\\|{}[]\"<>");
		while (word) {

			// make work upper case
			for (unsigned int i = 0; i < strlen(word); i++)
				word[i] = toupper(word[i]);

			// lookup word in dictionary (or make entry in dictionary
			// for this word) and add this module position (index) to
			// the word's associated list of module positions
			dictionary[lkey->Testament()-1][word].push_back(index);
			word = strtok(NULL, " !.,?;:()-=+/\\|{}[]\"<>");
		}
		free(wordBuf);
		(*this)++;
	}

	// reposition module back to where it was before we were called
	SetKey(*savekey);

	if (!savekey->Persist())
		delete savekey;

	if (searchkey)
		delete searchkey;

	
	// --------- Let's output an index from our dictionary -----------
	int datfd;
	int idxfd;
	map < string, list<long> >::iterator it;
	list<long>::iterator it2;
	unsigned long offset, entryoff;
	unsigned short size;

	string fname;
	fname = path;
	char ch = fname.c_str()[strlen(fname.c_str())-1];
	if ((ch != '/') && (ch != '\\'))
		fname += "/";

	// for old and new testament do...
	for (int loop = 0; loop < 2; loop++) {
		if ((datfd = open((fname + ((loop)?"ntwords.dat":"otwords.dat")).c_str(), O_CREAT|O_WRONLY|O_BINARY, 00644 )) == -1)
			return -1;
		if ((idxfd = open((fname + ((loop)?"ntwords.idx":"otwords.idx")).c_str(), O_CREAT|O_WRONLY|O_BINARY, 00644 )) == -1) {
			close(datfd);
			return -1;
		}

		// iterate thru each word in the dictionary
		for (it = dictionary[loop].begin(); it != dictionary[loop].end(); it++) {
			printf("%s: ", it->first.c_str());

			// get our current offset in our word.dat file and write this as the start
			// of the next entry in our database
			offset = lseek(datfd, 0, SEEK_CUR);
			write(idxfd, &offset, 4);

			// write our word out to the word.dat file, delineating with a \n
			write(datfd, it->first.c_str(), strlen(it->first.c_str()));
			write(datfd, "\n", 1);

			// force our mod position list for this word to be unique (remove
			// duplicates that may exist if the word was found more than once
			// in the verse
			it->second.unique();

			// iterate thru each mod position for this word and output it to
			// our database
			unsigned short count = 0;
			for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				entryoff= *it2;
				write(datfd, &entryoff, 4);
				count++;
			}
			
			// now see what our new position is in our word.dat file and
			// determine the size of this database entry
			size = lseek(datfd, 0, SEEK_CUR) - offset;

			// store the size of this database entry
			write(idxfd, &size, 2);
			printf("%d entries (size: %d)\n", count, size);
		}
		close(datfd);
		close(idxfd);
	}
	return 0;
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
 *	justCheckIfSupported	- if set, don't search, only tell if this
 *							function supports requested search.
 *
 * RET: listkey set to verses that contain istr
 */

ListKey &RawText::Search(const char *istr, int searchType, int flags, SWKey *scope, bool *justCheckIfSupported, void (*percent)(char, void *), void *percentUserData)
{
	listkey.ClearList();

	if ((fastSearch[0]) && (fastSearch[1])) {

		switch (searchType) {
		case -2: {

			if ((flags & REG_ICASE) != REG_ICASE)	// if haven't chosen to
											// ignore case
				break; // can't handle fast case sensitive searches

			// test to see if our scope for this search is bounded by a
			// VerseKey
			VerseKey *testKeyType = 0;
#ifndef _WIN32_WCE
			try {
#endif
				testKeyType = SWDYNAMIC_CAST(VerseKey, ((scope)?scope:key));
#ifndef _WIN32_WCE
			}
			catch ( ... ) {}
#endif
			// if we don't have a VerseKey * decendant we can't handle
			// because of scope.
			// In the future, add bool SWKey::isValid(const char *tryString);
			if (!testKeyType)
				break;


			// check if we just want to see if search is supported.
			// If we've gotten this far, then it is supported.
			if (justCheckIfSupported) {
				*justCheckIfSupported = true;
				return listkey;
			}

			SWKey saveKey = *testKeyType; // save current place

			char error = 0;
			char **words = 0;
			char *wordBuf = 0;
			int wordCount = 0;
			long start;
			unsigned short size;
			char *idxbuf = 0;
			char *datbuf = 0;
			list <long> indexes;
			list <long> indexes2;
			VerseKey vk;
			vk = TOP;

			(*percent)(10, percentUserData);

			// toupper our copy of search string
			stdstr(&wordBuf, istr);
			for (unsigned int i = 0; i < strlen(wordBuf); i++)
				wordBuf[i] = toupper(wordBuf[i]);

			// get list of individual words
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

			(*percent)(20, percentUserData);

			// clear our result set
			indexes.erase(indexes.begin(), indexes.end());

			// search both old and new testament indexes
			for (int j = 0; j < 2; j++) {
				// iterate thru each word the user passed to us.
				for (int i = 0; i < wordCount; i++) {

					// clear this word's result set
					indexes2.erase(indexes2.begin(), indexes2.end());
					error = 0;

					// iterate thru every word in the database that starts
					// with our search word
					for (int away = 0; !error; away++) {
						idxbuf = 0;
						
						// find our word in the database and jump ahead _away_
						error = fastSearch[j]->findoffset(words[i], &start, &size, away);

						// get the word from the database
						fastSearch[j]->getidxbufdat(start, &idxbuf);

						// check to see if it starts with our target word
						if (strlen(idxbuf) > strlen(words[i]))
							idxbuf[strlen(words[i])] = 0;
//						else	words[i][strlen(idxbuf)] = 0;
						if (!strcmp(idxbuf, words[i])) {

							// get data for this word from database
							free(idxbuf);
							idxbuf = (char *)calloc(size+2, 1);
							datbuf = (char *)calloc(size+2, 1);
							fastSearch[j]->gettext(start, size + 2, idxbuf, datbuf);

							// we know that the data consists of sizof(long)
							// records each a valid module position that constains
							// this word
							//
							// iterate thru each of these module positions
							long *keyindex = (long *)datbuf;
							while (keyindex < (long *)(datbuf + size - (strlen(idxbuf) + 1))) {
								if (i) {	// if we're not on our first word

									// check to see if this word is already in the result set.
									// This is our AND functionality
									if (find(indexes.begin(), indexes.end(), *keyindex) != indexes.end())
										// add to new result set
										indexes2.push_back(*keyindex);
								}
								else	indexes2.push_back(*keyindex);
								keyindex++;
							}
							free(datbuf);
						}
						else error = 1;	// no more matches
						free(idxbuf);
					}

					// make new result set final result set
					indexes = indexes2;

					percent((char)(20 + (float)((j*wordCount)+i)/(wordCount * 2) * 78), percentUserData);
				}

				// indexes contains our good verses, lets return them in a listkey
				indexes.sort();

				// iterate thru each good module position that meets the search
				for (list <long>::iterator it = indexes.begin(); it != indexes.end(); it++) {

					// set a temporary verse key to this module position
					vk.Testament(j+1);
					vk.Error();
					vk.Index(*it);

					// check scope
					// Try to set our scope key to this verse key
					if (scope) {
						*testKeyType = vk;

						// check to see if it set ok and if so, add to our return list
						if (*testKeyType == vk)
							listkey << (const char *) vk;
					}
					else listkey << (const char*) vk;
				}
			}
			(*percent)(98, percentUserData);

			free(words);
			free(wordBuf);

			*testKeyType = saveKey;	// set current place back to original

			listkey = TOP;
			(*percent)(100, percentUserData);
			return listkey;
		}

		default:
			break;
		}
	}

	// check if we just want to see if search is supported
	if (justCheckIfSupported) {
		*justCheckIfSupported = false;
		return listkey;
	}

	// if we don't support this search, fall back to base class
	return SWModule::Search(istr, searchType, flags, scope, justCheckIfSupported, percent, percentUserData);
}

#ifdef __VISUALC__
SWModule &RawText::operator =(SW_POSITION p) {
#else
RawText &RawText::operator =(SW_POSITION p) {
#endif
	SWModule::operator =(p);
	return *this;
}


SWModule &RawText::operator <<(const char *inbuf) {
	VerseKey *key = 0;
	// see if we have a VerseKey * or decendant
#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	// if we don't have a VerseKey * decendant, create our own
	if (!key)
		key = new VerseKey(this->key);

	settext(key->Testament(), key->Index(), inbuf);

	if (this->key != key) // free our key if we created a VerseKey
		delete key;

	return *this;
}


SWModule &RawText::operator <<(const SWKey *inkey) {
	VerseKey *destkey = 0;
	const VerseKey *srckey = 0;
	// see if we have a VerseKey * or decendant
#ifndef _WIN32_WCE
	try {
#endif
		destkey = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	// if we don't have a VerseKey * decendant, create our own
	if (!destkey)
		destkey = new VerseKey(this->key);

	// see if we have a VerseKey * or decendant
#ifndef _WIN32_WCE
	try {
#endif
		srckey = SWDYNAMIC_CAST(VerseKey, inkey);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	// if we don't have a VerseKey * decendant, create our own
	if (!srckey)
		srckey = new VerseKey(inkey);

	linkentry(destkey->Testament(), destkey->Index(), srckey->Index());

	if (this->key != destkey) // free our key if we created a VerseKey
		delete destkey;

	if (inkey != srckey) // free our key if we created a VerseKey
		delete srckey;

	return *this;
}


/******************************************************************************
 * RawFiles::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawText::deleteEntry() {

	VerseKey *key = 0;

#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	if (!key)
		key = new VerseKey(this->key);

	settext(key->Testament(), key->Index(), "");

	if (key != this->key)
		delete key;
}
