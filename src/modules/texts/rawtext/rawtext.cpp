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

RawText::RawText(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp) : SWText(iname, idesc, idisp), RawVerse(ipath)
{
	versebuf = 0;
	int datfd, idxfd;

	string fname;
	fname = path;
	char ch = fname.c_str()[strlen(fname.c_str())-1];
	if ((ch != '/') && (ch != '\\'))
		fname += "/";
	
	for (int loop = 0; loop < 2; loop++) {
		const char *fastidxname =(fname + ((loop)?"ntwords.dat":"otwords.dat")).c_str();
		datfd = open(fastidxname, O_RDONLY|O_BINARY);
		fastidxname = (fname + ((loop)?"ntwords.idx":"otwords.idx")).c_str();
		idxfd = open(fastidxname, O_RDONLY|O_BINARY);
		if ((datfd > 0) && (idxfd > 0)) {
			fastSearch[loop] = new RawStr((fname + ((loop)?"ntwords":"otwords")).c_str());
		}
		else	fastSearch[loop] = 0;
		close(idxfd);
		close(datfd);
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

RawText::operator char*()
{
	long  start;
	unsigned short size;
	VerseKey *key = 0;
	FilterList::iterator it;

	// see if we have a VerseKey * or decendant
	try {
		key = dynamic_cast<VerseKey *>(this->key);
	}
	catch ( ... ) {
	}
	// if we don't have a VerseKey * decendant, create our own
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);

	if (versebuf)
		delete [] versebuf;
	versebuf = new char [ ++size * 3 ];

	gettext(key->Testament(), start, size, versebuf);

	for (it = rawfilters.begin(); it != rawfilters.end(); it++) {
		(*it)->ProcessText(versebuf, size, key);
	}

	preptext(versebuf);
	RenderText(versebuf, size * 3);

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
	map < string, list<long> > dictionary[2];


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

	*this = TOP;

	VerseKey *lkey = (VerseKey *)key;
	while (!Error()) {
		long index = lkey->Index();
		wordBuf = (char *)calloc(sizeof(char), strlen(StripText()) + 1);
		strcpy(wordBuf, StripText());
		word = strtok(wordBuf, " !.,?;:()-=+/\\|{}[]\"<>");
		while (word) {
			for (unsigned int i = 0; i < strlen(word); i++)
				word[i] = toupper(word[i]);
			dictionary[lkey->Testament()-1][word].push_back(index);
			word = strtok(NULL, " !.,?;:()-=+/\\|{}[]\"<>");
		}
		free(wordBuf);
		(*this)++;
	}

	SetKey(*savekey);

	if (!savekey->Persist())
		delete savekey;

	if (searchkey)
		delete searchkey;

	
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

	for (int loop = 0; loop < 2; loop++) {
		if ((datfd = open((fname + ((loop)?"ntwords.dat":"otwords.dat")).c_str(), O_CREAT|O_WRONLY, 00644 /*S_IREAD|S_IWRITE*/)) == -1)
			return -1;
		if ((idxfd = open((fname + ((loop)?"ntwords.idx":"otwords.idx")).c_str(), O_CREAT|O_WRONLY, 00644 /*S_IREAD|S_IWRITE*/)) == -1) {
			close(datfd);
			return -1;
		}
		for (it = dictionary[loop].begin(); it != dictionary[loop].end(); it++) {
			printf("%s: ", it->first.c_str());
			offset = lseek(datfd, 0, SEEK_CUR);
			write(idxfd, &offset, 4);
			write(datfd, it->first.c_str(), strlen(it->first.c_str()));
			write(datfd, "\n", 1);
			it->second.unique();
			unsigned short count = 0;
			for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				entryoff= *it2;
				write(datfd, &entryoff, 4);
				count++;
			}
			size = lseek(datfd, 0, SEEK_CUR) - offset;
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

			if ((flags & REG_ICASE) != REG_ICASE) // can't handle fast case sensitive searches
				break;

			VerseKey *testKeyType = 0;
			try {
				testKeyType = dynamic_cast<VerseKey *>((scope)?scope:key);
			}
			catch ( ... ) {
			}
			// if we don't have a VerseKey * decendant we can handle because of scope.
			// In the future, add bool SWKey::isValid(const char *tryString);
			if (!testKeyType)
				break;


			// check if we just want to see if search is supported
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
			indexes.erase(indexes.begin(), indexes.end());

			// search both old and new testament indexes
			for (int j = 0; j < 2; j++) {
				// look up each word from index and see which verse indexes match every word.
				for (int i = 0; i < wordCount; i++) {
					indexes2.erase(indexes2.begin(), indexes2.end());
					error = 0;
					// increment to the next word and see if it starts with our search word
					for (int away = 0; !error; away++) {
						idxbuf = 0;
						error = fastSearch[j]->findoffset(words[i], &start, &size, away);
						fastSearch[j]->getidxbufdat(start, &idxbuf);
						if (strlen(idxbuf) > strlen(words[i]))
							idxbuf[strlen(words[i])] = 0;
						else	words[i][strlen(idxbuf)] = 0;
						if (!strcmp(idxbuf, words[i])) {
							free(idxbuf);
							idxbuf = (char *)calloc(size+1, 1);
							datbuf = (char *)calloc(size+1, 1);
							fastSearch[j]->gettext(start, size, idxbuf, datbuf);

							long *keyindex = (long *)datbuf;
							while (keyindex < (long *)(datbuf + size - (strlen(idxbuf) + 1))) {
								if (i) {
									if (find(indexes.begin(), indexes.end(), *keyindex) != indexes.end())
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
					indexes = indexes2;
					percent((char)(20 + (float)((j*wordCount)+i)/(wordCount * 2) * 78), percentUserData);
				}

				// indexes contains our good verses, lets return them in a listkey
				indexes.sort();
				for (list <long>::iterator it = indexes.begin(); it != indexes.end(); it++) {
					vk.Testament(j+1);
					vk.Error();
					vk.Index(*it);

					// check scope
					*testKeyType = vk;
					if (*testKeyType == vk)
						listkey << (const char *) vk;
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
	return SWModule::Search(istr, searchType, flags, scope, justCheckIfSupported, percent, percentUserData);
}


RawText &RawText::operator =(POSITION p) {
	SWModule::operator =(p);
	return *this;
}
