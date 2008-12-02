#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <fstream>

#include <utilstr.h>
#include <swmgr.h>
#include <rawtext.h>
#include <swbuf.h>
#include <utilxml.h>
#include <listkey.h>
#include <versekey.h>

#include <ztext.h>
#include <lzsscomprs.h>
#include <zipcomprs.h>
#include <cipherfil.h>

#ifdef _ICU_
#include <utf8nfc.h>
#include <latin1utf8.h>
#endif

//#define DEBUG

// Debug for simple transformation stack
//#define DEBUG_XFORM

// Debug for Words of Christ (aka WOC)
//#define DEBUG_QUOTE

// Debug for parsing osisRefs
//#define DEBUG_REF

// Debug for tag stack
//#define DEBUG_STACK

// Debug for titles
//#define DEBUG_TITLE

//Include all tags starting with the first div in the module
//#define INCLUDE_TAGS

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

using namespace std;

#ifdef _ICU_
UTF8NFC normalizer;
int normalized = 0;

Latin1UTF8 converter;
int converted = 0;
#endif

SWText *module = 0;
VerseKey currentVerse;
char activeOsisID[255];
char currentOsisID[255];

SWBuf activeVerseText;

ListKey currentKeyIDs = ListKey();

std::vector<ListKey> linkedVerses;

const char *osisabbrevs[] = {
	"Gen",    "Exod",   "Lev",    "Num",    "Deut",   "Josh",   "Judg",   "Ruth",
	"1Sam",   "2Sam",   "1Kgs",   "2Kgs",   "1Chr",   "2Chr",   "Ezra",   "Neh",
	"Esth",   "Job",    "Ps",     "Prov",   "Eccl",   "Song",   "Isa",    "Jer",
	"Lam",    "Ezek",   "Dan",    "Hos",    "Joel",   "Amos",   "Obad",   "Jonah",
	"Mic",    "Nah",    "Hab",    "Zeph",   "Hag",    "Zech",   "Mal",

	"Matt",   "Mark",   "Luke",   "John",   "Acts",   "Rom",    "1Cor",   "2Cor",
	"Gal",    "Eph",    "Phil",   "Col",    "1Thess", "2Thess", "1Tim",   "2Tim",
	"Titus",  "Phlm",   "Heb",    "Jas",    "1Pet",   "2Pet",   "1John",  "2John",
	"3John",  "Jude",   "Rev"
};

static bool inCanonicalOSISBook = true; // osisID is for a book that is not in Sword's canon
static bool normalize           = true; // Whether to normalize UTF-8 to NFC

bool isOSISAbbrev(const char *buf) {
	bool match = false;
	for (int i = 0; i < 66; i++) {
		if (!strcmp(buf, osisabbrevs[i])) {
			match = true;
			break;
		}
	}
	return match;
}

/**
 * Determine whether the string contains a valid unicode sequence.
 * The following table give the pattern of a valid UTF-8 character.
 * Unicode Range               1st       2nd       3rd       4th
 * U-00000000 - U-0000007F  0nnnnnnn
 * U-00000080 - U-000007FF  110nnnnn  10nnnnnn
 * U-00000800 - U-0000FFFF  1110nnnn  10nnnnnn  10nnnnnn
 * U-00010000 - U-001FFFFF  11110nnn  10nnnnnn  10nnnnnn  10nnnnnn
 * Note:
 *   1.  The latest UTF-8 RFC allows for a max of 4 bytes.
 *       Earlier allowed 6.
 *   2.  The number of bits of the leading byte before the first 0
 *       is the total number of bytes.
 *   3.  The "n" are the bits of the unicode codepoint.
 * This routine does not check to see if the code point is in the range.
 * It could.
 *
 * param  txt the text to check
 * return   1 if all high order characters form a valid unicode sequence
 *         -1 if there are no high order characters.
 *            Note: this is also a valid unicode sequence
 *          0 if there are high order characters that do not form
 *            a valid unicode sequence
 * author DM Smith
 */
int detectUTF8(const char *txt) {
	unsigned int  countUTF8 = 0;
	int           count     = 0;
	
	// Cast it to make masking and shifting easier
	const unsigned char *p = (const unsigned char*) txt;
	while (*p) {
		// Is the high order bit set?
		if (*p & 0x80) {
			// Then count the number of high order bits that are set.
			// This determines the number of following bytes
			// that are a part of the unicode character
			unsigned char i = *p;
			for (count = 0; i & 0x80; count++) {
				i <<= 1;
			}

			// Validate count:
			// Count 0: bug in code that would cause core walking
			// Count 1: is a pattern of 10nnnnnn,
			//          which does not signal the start of a unicode character
			// Count 5 to 8: 111110nn, 1111110n and 11111110 and 11111111
			//          are not legal starts, either
			if (count < 2 || count > 4) return 0;

			// At this point we expect (count - 1) following characters
			// of the pattern 10nnnnnn
			while (--count && *++p) {
				// The pattern of each following character must be: 10nnnnnn
				// So, compare the top 2 bits.
				if ((0xc0 & *p) != 0x80) return  0;
			}

			// Oops, we've run out of bytes too soon: Cannot be UTF-8
			if (count) return 0;

			// We have a valid UTF-8 character, so count it
			countUTF8++;
		}

		// Advance to the next character to examine.
		p++;
	}
	
	// At this point it is either UTF-8 or 7-bit ascii
	return countUTF8 ? 1 : -1;
}

// This routine converts an osisID or osisRef into one that SWORD can parse into a verse list
// An osisRef is made up of:
// a single osisID
// an osisID-osisID
// or
// an osisRef osisRef
//
// An osisID can have a work prefix which is terminated by a : and may have a grain
// which is started by a !
//
// However, SWORD cannot handle work prefixes or grains and expects ranges to be
// separated with a single;
void prepareSWVerseKey(SWBuf &buf) {
	// This routine modifies the buf in place
	char* s = buf.getRawData();
	char* p = s;
	bool inRange = false;
	while (*p) {
		if (inRange) {
#ifdef DEBUG_REF
		cout << "Copy range marker:" << *p << endl;;
#endif
			// Range markers are copied as is
			*s++ = *p++;
		}

		// Look ahead to see if we are in a work prefix
		// but don't look past an osisID
		char *n = p;
		while (*n && *n != ':' && *n != ' ' && *n != '-') {
			n++;
		}

		// We have found a work prefix
		if (*n == ':') {
			// set p to skip the work prefix
			p = n + 1;
#ifdef DEBUG_REF
			cout << "Found a work prefix ";
			for (char *x = s; x <= n; x++) {
				cout << *x;
			}
			cout << endl;
#endif
		}

		// Now we are in the meat of an osisID.
		// Copy it to its end but stop on a grain marker of '!'
#ifdef DEBUG_REF
		cout << "Copy osisID:";
#endif
		while (*p && *p != '!' && *p != ' ' && *p != '-') {
#ifdef DEBUG_REF
			cout << *p;
#endif
			*s++ = *p++;
		}
#ifdef DEBUG_REF
		cout << endl;
#endif

		// The ! and everything following until we hit
		// the end of the osisID is part of the grain reference
		if (*p == '!') {
			n = p;
			while (*n && *n != ' ' && *n != '-') {
				n++;
			}
#ifdef DEBUG_REF
			cout << "Found a grain suffix ";
			for (char *x = p; x < n; x++) {
				cout << *x;
			}
			cout << endl;
#endif
			p = n;
		}

		// At this point we have processed an osisID

		// if we are not in a range and the next characer is a -
		// then we are entering a range
		inRange = !inRange && *p == '-';

#ifdef DEBUG_REF
		if (inRange) {
			cout << "Found a range" << endl;
		}
#endif

		// between ranges and stand alone osisIDs we might have whitespace
		if (!inRange && *p == ' ') {
			// skip this and subsequent spaces
			while (*p == ' ') {
				p++;
			}
			// replacing them all with a ';'
			*s++ = ';';
#ifdef DEBUG_REF
			cout << "replacing space with ;. Remaining: " << p << endl;
#endif
		}
	}

	// Determine whether we have modified the buffer
	// We have modified the buffer if s is not sitting on the null byte of the original
	if (*s) {
		// null terminate the reference
		*s = '\0';
		// Since we modified the swbuf, we need to tell it what we have done
		buf.setSize(s - buf.c_str());
#ifdef DEBUG_REF
		cout << "shortended keyVal to`" << buf.c_str() << "`"<< endl;
#endif
	}
}

bool isKJVRef(const char *buf) {
	VerseKey vk, test;
	vk.AutoNormalize(0);
	vk.Headings(1);	// turn on mod/testmnt/book/chap headings
	vk.Persist(1);
	// lets do some tests on the verse --------------
	vk = buf;
	test = buf;

	if (vk.Testament() && vk.Book() && vk.Chapter() && vk.Verse()) { // if we're not a heading
#ifdef DEBUG
		cout << (const char*)vk << " == "  << (const char*)test << endl;
#endif
		return (vk == test);
	}
	else return true;	// no check if we're a heading... Probably bad.
}


/**
 * This routine is used to ensure that all the text in the input is saved to the module.
 * Assumption: The input orders all the verses for a chapter in numerical order. Thus, any
 * verses that are not in the KJV versification (v11n) follow those that are.
 *
 * The prior implementation of this adjusted the verse to the last one that is in the KJV v11n.
 * If it the chapter were extra, then it is appended to the last verse of the last
 * chapter in the KJV v11n for that book. If it is just extra verses for a chapter, then it is
 * appended to the last verse of the chapter.
 *
 * The problem with this is when a OSIS verse refers to more than one KJV v11n verse, e.g.
 * osisID="Gen.1.29 Gen.1.30 Gen.1.31" (Gen.1.31 is the KJV v11n last verse) and then it is
 * followed by Gen.1.31. In this case, we need to append Gen.1.29 and make sure that Gen.1.30
 * and Gen.1.31 link to the new Gen.1.29.
 *
 * This routine assumes that linking is postponed to the end so that in the example Gen.1.30-31
 * are not linked but rather empty. This routine will then find the last verse in the computed
 * chapter that has content.
 *
 * Alternative, we could have done linking as we went, but this routine would have needed
 * to find the first entry in the link set and elsewhere in the code when appending to a
 * verse, it would need to be checked for adjacent links and those would have needed to be adjusted.
 *
 * param key the key that may need to be adjusted
 */
void makeKJVRef(VerseKey &key) {
	VerseKey saveKey;
	saveKey.AutoNormalize(0);
	saveKey.Headings(1);
	saveKey = currentVerse;

	cout << "re-versified " << key;
#ifdef DEBUG
	cout << "\tC" << (int)(key.getChapterMax()) << ":V" << (int)(key.getVerseMax());
#endif
	if (key.Chapter() > key.getChapterMax()) {
		key.Chapter(key.getChapterMax());
		key.Verse(key.getVerseMax());
	}
	else if (key.Verse() > key.getVerseMax()) {
		key.Verse(key.getVerseMax());
	}

	currentVerse = key;
	currentVerse.ClearBounds();
	currentVerse--;
	while (!currentVerse.Error() && currentVerse.getVerse() > 0) {
		SWBuf currentText = module->getRawEntry();
		if (currentText.length()) {
			break;
		}
		currentVerse--;
	}

	key = currentVerse;
	cout << "\tas " << key << endl;

	currentVerse = saveKey;
}

void writeEntry(SWBuf &text, bool force = false) {
	char keyOsisID[255];

	static const char* revision = "<milestone type=\"x-importer\" subType=\"x-osis2mod\" n=\"$Rev:$\"/>";
	static bool firstOT = true;
	static bool firstNT = true;

	if (!inCanonicalOSISBook) {
		return;
	}

	strcpy(keyOsisID, currentVerse.getOSISRef());

	// set keyOsisID to anything that an osisID cannot be.
	if (force) {
		strcpy(keyOsisID, "-force");
	}

	static VerseKey lastKey;
	lastKey.AutoNormalize(0);
	lastKey.Headings(1);

	VerseKey saveKey;
	saveKey.AutoNormalize(0);
	saveKey.Headings(1);
	saveKey = currentVerse;

	// If we have seen a verse and the supplied one is different then we output the collected one.
	if (*activeOsisID && strcmp(activeOsisID, keyOsisID)) {

		currentVerse = lastKey;

		if (!isKJVRef(currentVerse)) {
			makeKJVRef(currentVerse);
		}

#ifdef _ICU_
		int utf8State = detectUTF8(activeVerseText.c_str());
		if (normalize) {
			// Don't need to normalize text that is ASCII
			// But assume other non-UTF-8 text is Latin1 (cp1252) and convert it to UTF-8
			if (!utf8State) {
				cout << "Warning: " << activeOsisID << ": Converting to UTF-8 (" << activeVerseText << ")" << endl;
				converter.processText(activeVerseText, (SWKey *)2);  // note the hack of 2 to mimic a real key. TODO: remove all hacks
				converted++;

				// Prepare for double check. This probably can be removed.
				// But for now we are running the check again.
				// This is to determine whether we need to normalize output of the conversion.
				utf8State = detectUTF8(activeVerseText.c_str());
			}

			// Double check. This probably can be removed.
			if (!utf8State) {
				cout << "Error: " << activeOsisID << ": Converting to UTF-8 (" << activeVerseText << ")" << endl;
			}

			if (utf8State > 0) {
				SWBuf before = activeVerseText;
				normalizer.processText(activeVerseText, (SWKey *)2);  // note the hack of 2 to mimic a real key. TODO: remove all hacks
				if (before != activeVerseText) {
					normalized++;
				}
			}
		}
#endif

		// Put the revision into the module
		int testmt = currentVerse.Testament();
		if ((testmt == 1 && firstOT) || (testmt == 2 && firstNT)) {
			VerseKey t;
			t.AutoNormalize(0);
			t.Headings(1);
			t = currentVerse;
			currentVerse.Book(0);
			currentVerse.Chapter(0);
			currentVerse.Verse(0);
			module->setEntry(revision);
			currentVerse = t;
			switch (testmt) {
			case 1:
				firstOT = false;
				break;
			case 2:
				firstNT = false;
				break;
			}
                }

		// If the entry already exists, then append this entry to the text.
		// This is for verses that are outside the KJV versification. They are appended to the prior verse.
		// The space should not be needed if we retained verse tags.
		SWBuf currentText = module->getRawEntry();
		if (currentText.length()) {
			cout << "Appending entry: " << currentVerse.getOSISRef() << ": " << activeVerseText << endl;
			activeVerseText = currentText + " " + activeVerseText;
		}

#ifdef DEBUG
		cout << "Write: " << activeOsisID << ":" << currentVerse.getOSISRef() << ": " << activeVerseText << endl;
#endif

		module->setEntry(activeVerseText);
		activeVerseText = "";
	}

	// The following is for initial verse content and for appending interverse content.
	// Eliminate leading whitespace on the beginning of each verse and
	// before we append to current content, since we just added one
	text.trimStart();
	if (activeVerseText.length()) {
		activeVerseText += " ";
		activeVerseText += text;
	}
	else {
		activeVerseText = text;
	}
	// text has been consumed so clear it out.
	text = "";

	currentVerse = saveKey;
	lastKey = currentVerse;
	strcpy(activeOsisID, keyOsisID);
}


void linkToEntry(VerseKey& linkKey, VerseKey& dest) {

	// Only link verses that are in the KJV versification.
	if (!isKJVRef(linkKey)) {
		return;
	}

	VerseKey saveKey;
	saveKey.AutoNormalize(0);
	saveKey.Headings(1);
	saveKey = currentVerse;
	currentVerse = linkKey;

	cout << "Linking " << module->KeyText() << " to " << dest.getText() << "\n";
	module->linkEntry(&dest);

	currentVerse = saveKey;
}

// Return true if the content was handled or is to be ignored.
//		false if the what has been seen is to be accumulated and considered later.
bool handleToken(SWBuf &text, XMLTag token) {

	// Everything between the begin book tag and the first begin chapter tag is inBookHeader
	static bool               inBookHeader    = false;

	// Everything between the begin chapter tag and the first begin verse tag is inChapterHeader
	static bool               inChapterHeader = false;

	// Flags indicating whether we are processing the content of a verse
	static bool               inVerse         = false;

	// Used to remember titles that need to be handle specially
	static SWBuf              header          = "";
	static SWBuf              lastTitle       = "";
	static int                titleOffset     = -1;
	static bool               inTitle         = false;
	static int                titleDepth      = 0;

	// Flag indicating whether we are in "Words of Christ"
	static bool               inWOC           = false;
	// Tag for WOC quotes within a verse
	static XMLTag             wocTag          = "<q who=\"Jesus\" marker=\"\">";

	// Flag used to indicate where useful text begins
	static bool               firstDiv        = false;

	// Stack of quote elements used to handle Words of Christ
	static std::stack<XMLTag> quoteStack;

	// Stack of elements used to validate that books, chapters and verses are well-formed
	// This goes beyond simple xml well-formed and also considers milestoned div, chapter and verse
	// to be begin and end tags, too.
	// It is an error if books and chapters are not well formed (though not required by OSIS)
	// It is a warning that verses are not well formed (because some clients are not ready)
	static std::stack<XMLTag> tagStack;

	// The following are used to validate well-formedness
	static int                chapterDepth    = 0;
	static int                bookDepth       = 0;
	static int                verseDepth      = 0;

	int                       tagDepth        = tagStack.size();
	const char               *tokenName       = token.getName();
	bool                      isEndTag        = token.isEndTag() || token.getAttribute("eID");
	const char               *typeAttr        = token.getAttribute("type");

	//Titles are treated specially.
	// If the title has an attribute type of "main" or "chapter"
	// it belongs to its <div> or <chapter> and is treated as part of its heading
	// Otherwise if it a title in a chapter before the first the first verse it
	// is put into the verse as a preverse title.
	if (!inVerse) {
	if (!token.isEmpty() && !isEndTag && titleDepth == 0 && (!strcmp(tokenName, "title")) && (!typeAttr || (strcmp(typeAttr, "main") && strcmp(typeAttr, "chapter")))) {
		titleOffset = text.length(); //start of the title tag
		lastTitle = "";
		inTitle = true;
		tagStack.push(token);
#ifdef DEBUG_STACK
		cout << currentOsisID << ": push (" << tagStack.size() << ") " << token.getName() << endl;
#endif
		titleDepth = tagStack.size();
		return false;
	}

	// Check titleDepth since titles can be nested. Don't want to quit too early.
	else if (inTitle && isEndTag && tagDepth == titleDepth && (!strcmp(tokenName, "title"))) {
		lastTitle.append(text.c_str() + titleOffset); //<title ...> up to the end </title>
		lastTitle.append(token); //</title>

#ifdef DEBUG_TITLE
		cout << currentOsisID << ":" << endl;
		cout << "\tlastTitle:      " << lastTitle.c_str() << endl;
		cout << "\ttext-lastTitle: " << text.c_str()+titleOffset << endl;
		cout << "\ttext:	   " << text.c_str() << endl;
#endif
		inTitle = false;
		titleDepth = 0;
#ifdef DEBUG_STACK
		cout << currentOsisID << ": pop(" << tagStack.size() << ") " << tagStack.top()->getName() << endl;
#endif
		tagStack.pop();
		return false; // don't add </title> to the text itself
	}
	}


//-- START TAG -------------------------------------------------------------------------

	if (!isEndTag) {

		// Remember non-empty start tags
		if (!token.isEmpty()) {
			tagStack.push(token);
#ifdef DEBUG_STACK
			cout << currentOsisID << ": push (" << tagStack.size() << ") " << token.getName() << endl;
#endif
		}

		// throw away everything up to the first div
		if (!firstDiv && !strcmp(tokenName, "div")) {
			firstDiv = true;
#ifdef DEBUG
			cout << "Found first div and pitching prior material: " << text << endl;
#endif
			text     = "";
		}

		//-- WITH OSIS ID      -------------------------------------------------------------------------
		//--   OR ANNOTATE REF -------------------------------------------------------------------------
		if (token.getAttribute("osisID") || token.getAttribute("annotateRef")) {

			// BOOK START, <div type="book" ...>
			if ((!strcmp(tokenName, "div")) && (typeAttr && !strcmp(typeAttr, "book"))) {
				inVerse = false;
				if (inBookHeader || inChapterHeader) {	// this one should never happen, but just in case
#ifdef DEBUG_TITLE
					cout << currentOsisID << ": HEADING ";
#endif
					currentVerse.Testament(0);
					currentVerse.Book(0);
					currentVerse.Chapter(0);
					currentVerse.Verse(0);
					writeEntry(text);
				}
				// Initializing a temporary and copying that because there were problems with setting the text directly
				VerseKey t;
				t.AutoNormalize(0);
				t.Headings(1);
				t = token.getAttribute("osisID");
				currentVerse = t;
				currentVerse.Chapter(0);
				currentVerse.Verse(0);
				strcpy(currentOsisID, currentVerse.getOSISRef());
				inBookHeader = true;
				inChapterHeader = false;
				lastTitle = "";
				bookDepth = tagStack.size();
				chapterDepth = 0;
				verseDepth = 0;

				inCanonicalOSISBook = isOSISAbbrev(token.getAttribute("osisID"));
#ifdef DEBUG
				cout << "Current book is " << currentVerse << (!inCanonicalOSISBook ? "not in KJV, ignoring" : "") << endl;
#endif

#ifndef INCLUDE_TAGS
				return true;
#else
				return false;
#endif
			}

			// CHAPTER START, <div type="chapter" ...> or <chapter ...>
			if (((!strcmp(tokenName, "div")) && (typeAttr && !strcmp(typeAttr, "chapter"))) ||
			     (!strcmp(tokenName, "chapter"))
			   ) {
				inVerse = false;
				if (inBookHeader) {
#ifdef DEBUG_TITLE
					cout << currentOsisID << ": BOOK HEADING "<< text.c_str() << endl;
#endif
					writeEntry(text);
				}

				// I don't know why, but I cannot do the following,
				// as it does not change the content of VerseKey!
				// currentVerse = token.getAttribute("osisID");
				VerseKey t;
				t.AutoNormalize(0);
				t.Headings(1);
				t = token.getAttribute("osisID");
				currentVerse = t;
				currentVerse.Verse(0);
#ifdef DEBUG
				cout << "Current chapter is " << currentVerse << " (" << token.getAttribute("osisID") << ")" << endl;
#endif
				strcpy(currentOsisID, currentVerse.getOSISRef());
				inBookHeader = false;
				inChapterHeader = true;
				lastTitle = "";
				chapterDepth = tagStack.size();
				verseDepth = 0;

#ifndef INCLUDE_TAGS
				return true;
#else
				return false;
#endif
			}

			// VERSE, <verse ...> OR COMMENTARY START, <div annotateType="xxx" ...>
			if (!strcmp(tokenName, "verse") ||
			   (!strcmp(tokenName, "div") && token.getAttribute("annotateType"))) {
#ifdef DEBUG
				cout << "Entering verse" << endl;
#endif
				inVerse = true;
				if (inChapterHeader) {
					SWBuf heading = text;

					//make sure we don't insert the preverse title which belongs to the first verse of this chapter!
					// Did we have a preverse title?
					if (lastTitle.length()) {
						//Was the preVerse title in the header (error if not)?
						const char* header = heading.c_str();
						const char* preVerse = strstr(header, lastTitle);
						if (preVerse) {
							if (preVerse == header) {
								heading = ""; // do nothing
							}
							else {
								// remove everything before the title from the beginning.
								text = preVerse;
								// Remove text from the end of the header.
								heading.setSize(preVerse - header);
							}
						}
						else {
							cout << currentOsisID << ": Warning: Bug in code. Could not find title." << endl;
						}
					}
					else {
						text = "";
					}


					if (heading.length()) {
#ifdef DEBUG_TITLE
						cout << currentOsisID << ": CHAPTER HEADING "<< heading.c_str() << endl;
#endif
						writeEntry(heading);
					}

					inChapterHeader = false;
				}

				SWBuf keyVal = token.getAttribute(strcmp(tokenName, "verse") ? "annotateRef" : "osisID");
				// The osisID or annotateRef can be more than a single verse
				// The first or only one is the currentVerse
				prepareSWVerseKey(keyVal);
				ListKey verseKeys = currentVerse.ParseVerseList(keyVal, currentVerse, true);
				int memberKeyCount = verseKeys.Count();
				if (memberKeyCount) {
					currentVerse = verseKeys.getElement(0);
					// See if this osisID or annotateRef refers to more than one verse.
					// If it does, save it until all verses have been seen.
					// At that point we will output links.
					if (memberKeyCount > 1) {
						linkedVerses.push_back(verseKeys);
					}
				}
				else {
					cout << "Error: Invalid osisID/annotateRef: " << token.getAttribute(strcmp(tokenName, "verse") ? "annotateRef" : "osisID") << endl;
				}

				strcpy(currentOsisID, currentVerse.getOSISRef());
#ifdef DEBUG
				cout << "Current verse is " << currentVerse << endl;
				cout << "osisID/annotateRef is adjusted to: " << keyVal << endl;
#endif

				verseDepth = tagStack.size();

#ifdef INCLUDE_TAGS
				text.append(token);
#endif
				if (inWOC) {
					text.append(wocTag);
				}
				return true;
			}
		}

		// Handle WOC quotes.
		// Note this requires transformBSP to make them into milestones
		// Otherwise have to do it here
		if (!strcmp(tokenName, "q")) {
			
			quoteStack.push(token);
#ifdef DEBUG_QUOTE
			cout << currentOsisID << ": quote top(" << quoteStack.size() << ") " << token << endl;
#endif
			if (token.getAttribute("who") && !strcmp(token.getAttribute("who"), "Jesus")) {
				inWOC = true;
				text.append(wocTag);
				// This puts the quote marks in the WOC
				token.setAttribute("who", 0); // remove the who="Jesus"
				text.append(token);
				return true;
			}
			return false;
		}

		// Handle stuff between the verses
		// Whitespace producing empty tokens are appended to prior entry
		// Also the quote
		// This is a hack to get ESV to work
		// Don't write if there is not a valid osisID yet.
		if (!inTitle && !inVerse && token.isEmpty() && strcmp(currentOsisID, "N/A")) {
			if (!strcmp(tokenName, "div") ||
			    !strcmp(tokenName, "q")   ||
			    !strcmp(tokenName, "l")   ||
			    !strcmp(tokenName, "lb")  ||
			    !strcmp(tokenName, "lg")
			   ) {
#ifdef DEBUG
				cout << currentOsisID << ": appending interverse start token " << token << ":" << text.c_str() << endl;
#endif
				SWBuf tmp = token.toString();
				writeEntry(tmp);
				return true;
			}
#ifdef DEBUG
			cout << currentOsisID << ": interverse start token " << token << ":" << text.c_str() << endl;
#endif
		}
	}

//-- EMPTY and END TAG ---------------------------------------------------------------------------------------------

	else {

		if (tagStack.empty()) {
			cout << currentOsisID << ": tag expected" << endl;
			exit(1);
		}

		if (!token.isEmpty()) {
			XMLTag topToken = tagStack.top();
			tagDepth = tagStack.size();
#ifdef DEBUG_STACK
			cout << currentOsisID << ": pop(" << tagDepth << ") " << topToken.getName() << endl;
#endif
			tagStack.pop();

			if (strcmp(topToken.getName(), tokenName)) {
				cout << "Error: " << currentOsisID << ": Expected " << topToken.getName() << " found " << tokenName << endl;
//				exit(1);	// I'm sure this validity check is a good idea, but there's a but somewhere that's killing the converter here.
						// So I'm disabling this line. Unvalidated OSIS files shouldn't be run through the converter anyway.
			}
		}

		// VERSE and COMMENTARY END
		if (!strcmp(tokenName, "verse") || (inVerse && !strcmp(tokenName, "div"))) {
			inVerse = false;

			if (tagDepth != verseDepth) {
				cout << "Warning verse " << currentOsisID << " is not well formed:(" << verseDepth << "," << tagDepth << ")" << endl;
			}

			if (lastTitle.length()) {
				const char* end = strchr(lastTitle, '>');
#ifdef DEBUG_TITLE
				cout << currentOsisID << ":" << endl;
				cout << "\t" << lastTitle << endl;
	 			cout << "\tlength=" << int(end+1 - lastTitle.c_str()) << ", tag:" << lastTitle.c_str() << endl;
#endif

				SWBuf titleTagText;
				titleTagText.append(lastTitle.c_str(), end+1 - lastTitle.c_str());
#ifdef DEBUG_TITLE
				cout << currentOsisID << ": tagText: " << titleTagText.c_str() << endl;;
#endif

				XMLTag titleTag(titleTagText);
				titleTag.setAttribute("type", "section");
				titleTag.setAttribute("subType", "x-preverse");

				//we insert the title into the text again - make sure to remove the old title text
				const char* pos = strstr(text, lastTitle);
				if (pos) {
					SWBuf temp;
					temp.append(text, pos-text.c_str());
					temp.append(pos+lastTitle.length());
					text = temp;
				}

				//if a title was already inserted at the beginning insert this one after that first title
				int titlePos = 0;
				if (!strncmp(text.c_str(),"<title ",7)) {
					const char* tmp = strstr(text.c_str(), "</title>");
					if (tmp) {
						titlePos = (tmp-text.c_str()) + 8;
					}
				}
				text.insert(titlePos, end+1);
				text.insert(titlePos, titleTag);
			}

			// If we are in WOC then we need to terminate the <q who="Jesus" marker=""> that was added earlier in the verse.
			if (inWOC) {
				text.append("</q>");
			}
#ifdef INCLUDE_TAGS
			text.append(token);
#endif
			writeEntry(text);

			lastTitle = "";
			verseDepth = 0;
			return true;
		}
		
		// Handle WOC quotes.
		// Note this requires transformBSP to make them into milestones
		// Otherwise have to manage it here
		if (!strcmp(tokenName, "q")) {
			XMLTag topToken = quoteStack.top();
#ifdef DEBUG_QUOTE
			cout << currentOsisID << ": quote pop(" << quoteStack.size() << ") " << topToken << " -- " << token << endl;
#endif
			quoteStack.pop();

			// If we have found an end tag for a <q who="Jesus"> then we are done with the WOC
			// and we need to terminate the <q who="Jesus" marker=""> that was added earlier in the verse.
			if (token.getAttribute("who") && !strcmp(token.getAttribute("who"), "Jesus")) {
#ifdef DEBUG_QUOTE
				cout << currentOsisID << ": (" << quoteStack.size() << ") " << topToken << " -- " << token << endl;
#endif
				inWOC = false;
				const char *sID = topToken.getAttribute("sID");
				const char *eID = token.getAttribute("eID");
				if (!sID) {
					sID = "";
				}
				if (!eID) {
					eID = "";
				}
				if (strcmp(sID, eID)) {
					cout << "Error: improper nesting " << currentVerse << ": matching (sID,eID) not found (" << sID << "," << eID << ")" << endl;
				}

				// This puts the quote marks in the WOC
				text.append(token);
				text.append("</q>");
				return true;
			}
			return false;
		}

		if (!inTitle && !inVerse && !inBookHeader && !inChapterHeader) {
			// Is this the end of a chapter.
			if (tagDepth == chapterDepth && (!strcmp(tokenName, "div") || !strcmp(tokenName, "chapter"))) {
#ifdef INCLUDE_TAGS
				text.append(token);
				writeEntry(text);
#endif
				chapterDepth = 0;
				verseDepth = 0;
				return true;
			}

			// Is it the end of a book
			if (tagDepth == bookDepth && (!strcmp(tokenName, "div"))) {
#ifdef INCLUDE_TAGS
				text.append(token);
				writeEntry(text);
#endif
				bookDepth = 0;
				chapterDepth = 0;
				verseDepth = 0;
				return true;
			}

			// OTHER MISC END TAGS WHEN !INVERSE
			// Test that is between verses, or after the last is appended to the preceeding verse.
			if (!strcmp(tokenName, "div") ||
			    !strcmp(tokenName, "chapter") ||
			    !strcmp(tokenName, "q")   ||
			    !strcmp(tokenName, "l")   ||
			    !strcmp(tokenName, "lb")  ||
			    !strcmp(tokenName, "lg")
			   ) {
				text.append(token);
				writeEntry(text);
#ifdef DEBUG
				cout << currentOsisID << ": appending interverse end tag: " << tokenName << "(" << tagDepth << "," << chapterDepth << "," << bookDepth << ")" << endl;
#endif
				return true;
			}

			// Is it the end of an osis document
			if (!strcmp(tokenName, "osisText") || !strcmp(tokenName, "osis")) {
				bookDepth = 0;
				chapterDepth = 0;
				verseDepth = 0;
				text = "";
				return true;
			}
#ifdef DEBUG
			cout << currentOsisID << ": interverse end tag: " << tokenName << "(" << tagDepth << "," << chapterDepth << "," << bookDepth << ")" << endl;
#endif
		}
	}
	return false;
}

/**
 * Support normalizations necessary for a SWORD module.
 * OSIS allows for document structure (Book, Section, Paragraph or BSP)
 * to overlap Bible versification (Book, Chapter, Verse).
 * Most SWORD applications need to display verses in isolation or in HTML table cells,
 * requiring each stored entry (i.e. verses) to be well-formed xml.
 * This routine normalizes container elements which could cross verse boundaries into milestones.
 * For most of these OSIS elements, there is a milestone form. However, p is not milestoneable.
 * For this reason, p is transformed into lb elements.
 * param t the tag to transform
 * return the transformed tag or the original one
 */
XMLTag transformBSP(XMLTag t) {
	static std::stack<XMLTag> bspTagStack;
	static int sID = 1;
	char buf[11];

	// Support simplification transformations
	if (t.isEmpty()) {
#ifdef DEBUG_XFORM
		cout << currentOsisID << ": xform empty " << t << endl;
#endif
		return t;
	}

	const char* tagName = t.getName();
	if (!t.isEndTag()) {
		// Transform <q> into <q sID=""/> except for <q who="Jesus">
		if ((!strcmp(tagName, "q")) && (!t.getAttribute("who") || strcmp(t.getAttribute("who"), "Jesus"))) {
			t.setEmpty(true);
			sprintf(buf, "gen%d", sID++);
			t.setAttribute("sID", buf);
		}

		// Transform <p> into <lb type="x-begin-paragraph"/>
		else if (!strcmp(tagName, "p")) {
			// note there is no process that should care about type, it is there for reversability
			t.setText("<lb type=\"x-begin-paragraph\" />");
		}

		// Transform <tag> into <tag  sID="">, where tag is a milestoneable element.
		// The following containers are milestoneable.
		// abbr, closer, div, foreign, l, lg, salute, signed, speech
		// Leaving out:
		//   abbr	When would this ever cross a boundary?
		//   seg	as it is used for a divineName hack
		//   foreign	so that it can be easily italicized
		else if (!strcmp(tagName, "chapter") ||
			 !strcmp(tagName, "closer")  ||
			 !strcmp(tagName, "div")     ||
			 !strcmp(tagName, "l")       ||
			 !strcmp(tagName, "lg")      ||
			 !strcmp(tagName, "salute")  ||
			 !strcmp(tagName, "signed")  ||
			 !strcmp(tagName, "speech")
			) {
			t.setEmpty(true);
			sprintf(buf, "gen%d", sID++);
			t.setAttribute("sID", buf);
		}
		bspTagStack.push(t);
#ifdef DEBUG_XFORM
		cout << currentOsisID << ": xform push (" << bspTagStack.size() << ") " << t << " (tagname=" << tagName << ")" << endl;
		XMLTag topToken = bspTagStack.top();
		cout << currentOsisID << ": xform top(" << bspTagStack.size() << ") " << topToken << endl;
#endif
	}
	else {
		XMLTag topToken = bspTagStack.top();
#ifdef DEBUG_XFORM
		cout << currentOsisID << ": xform pop(" << bspTagStack.size() << ") " << topToken << endl;
#endif
		bspTagStack.pop();

		// If we have found an end tag for a <q> that was transformed then transform this one as well.
		if ((!strcmp(tagName, "q")) && (!strcmp(topToken.getName(), "q")) && (!topToken.getAttribute("who") || strcmp(topToken.getAttribute("who"), "Jesus"))) {
			// make this a clone of the start tag with sID changed to eID
			t = topToken;
			t.setAttribute("eID", t.getAttribute("sID"));
			t.setAttribute("sID", 0);
		}

		// Look for paragraph tags.
		// If we have found an end tag for a <p> that was transformed then transform this as well.
		else if ((!strcmp(tagName, "p")) && (!strcmp(topToken.getName(), "lb"))) {
			t.setText("<lb type=\"x-end-paragraph\" />");
		}

		// Look for the milestoneable container tags handled above.
		else if (!strcmp(tagName, "chapter") ||
			 !strcmp(tagName, "closer")  ||
			 !strcmp(tagName, "div")     ||
			 !strcmp(tagName, "l")       ||
			 !strcmp(tagName, "lg")      ||
			 !strcmp(tagName, "salute")  ||
			 !strcmp(tagName, "signed")  ||
			 !strcmp(tagName, "speech")
			) {
			// make this a clone of the start tag with sID changed to eID
			t = topToken;
			t.setAttribute("eID", t.getAttribute("sID"));
			t.setAttribute("sID", 0);
		}
	}

	return t;
}

/**
 * Write out all links in the module.
 * Waiting is necessary because writeEntry might ultimately append
 * text to a verse moving it's offset in the data file.
 * While we are minimizing it by postponing the write until we have
 * gathered the next verse, the following scenario is happening:
 * A module is using linked verses and has some verses that are not
 * in the KJV versification. If the out-of-canon verse happens following
 * a linked verse, the out-of-canon verse is appended to the prior
 * verse. Care has to be taken that the linked verses all point to
 * the first of the set.
 */
void writeLinks()
{
	// Link all the verses
	VerseKey destKey;
	destKey.AutoNormalize(0);
	destKey.Headings(1);

	VerseKey linkKey;
	linkKey.AutoNormalize(0);
	linkKey.Headings(1);
	for (unsigned int i = 0; i < linkedVerses.size(); i++) {
		// The verseKeys is an list of verses
		// where the first is the real verse
		// and the others link to it.
		ListKey verseKeys = linkedVerses[i];
		for (verseKeys = TOP; !verseKeys.Error(); verseKeys++) {
			destKey = verseKeys;
		}
		verseKeys.setPosition(TOP);
		destKey = verseKeys.getElement();
		destKey.ClearBounds();
		verseKeys.increment(1);

		while (!verseKeys.Error()) {
			linkKey = verseKeys.getElement();
			linkKey.ClearBounds();
			verseKeys.increment(1);
			linkToEntry(linkKey, destKey);
		}
	}
}

void usage(const char *app, const char *error = 0) {

	if (error) fprintf(stderr, "\n%s: %s\n", app, error);

	fprintf(stderr, "\nusage: %s <output/path> <osisDoc> [OPTIONS]\n", app);
	fprintf(stderr, "  -a\t\t\t augment module if exists (default is to create new)\n");
	fprintf(stderr, "  -z\t\t\t use ZIP compression (default no compression)\n");
	fprintf(stderr, "  -Z\t\t\t use LZSS compression (default no compression)\n");
	fprintf(stderr, "  -b <2|3|4>\t\t compression block size (default 4):\n");
	fprintf(stderr, "\t\t\t\t 2 - verse; 3 - chapter; 4 - book\n");
	fprintf(stderr, "  -c <cipher_key>\t encipher module using supplied key\n");
	fprintf(stderr, "\t\t\t\t (default no enciphering)\n");
	fprintf(stderr, "  -N\t\t\t Do not convert UTF-8 or normalize UTF-8 to NFC\n");
	fprintf(stderr, "\t\t\t\t (default is to convert to UTF-8, if needed, and then normalize to NFC");
	fprintf(stderr, "\t\t\t\t Note: all UTF-8 texts should be normalized to NFC\n");
	exit(-1);
}

int main(int argc, char **argv) {

	fprintf(stderr, "You are running osis2mod: $Rev$\n");

	// Let's test our command line arguments
	if (argc < 3) {
		usage(*argv);
	}

	// variables for arguments, holding defaults
	const char* program = argv[0];
	const char* path    = argv[1];
	const char* osisDoc = argv[2];
	int append          = 0;
	int compType        = 0;
	int iType           = 4;
	string cipherKey    = "";

	SWCompress *compressor = 0;

	for (int i = 3; i < argc; i++) {
		if (!strcmp(argv[i], "-a")) {
			append = 1;
		}
		else if (!strcmp(argv[i], "-z")) {
			if (compType) usage(*argv, "Cannot specify both -z and -Z");
			compType = 2;
		}
		else if (!strcmp(argv[i], "-Z")) {
			if (compType) usage(*argv, "Cannot specify both -z and -Z");
			compType = 1;
		}
		else if (!strcmp(argv[i], "-b")) {
			if (i+1 < argc) {
				iType = atoi(argv[++i]);
				if ((iType >= 2) && (iType <= 4)) continue;
			}
			usage(*argv, "-b requires one of <2|3|4>");
		}
		else if (!strcmp(argv[i], "-N")) {
			normalize = false;
		}
		else if (!strcmp(argv[i], "-c")) {
			if (i+1 < argc) cipherKey = argv[++i];
			else usage(*argv, "-c requires <cipher_key>");
		}
		else usage(*argv, (((SWBuf)"Unknown argument: ")+ argv[i]).c_str());
	}

	switch (compType) {	// these are deleted by zText
		case 0: break;
		case 1: compressor = new LZSSCompress(); break;
		case 2: compressor = new ZipCompress(); break;
	}

#ifndef _ICU_
	if (normalize) {
		normalize = false;
		cout << program << " is not compiled with support for ICU. Ignoring -n flag." << endl;
	}
#endif

#ifdef DEBUG
	cout << "path: " << path << " osisDoc: " << osisDoc << " create: " << append << " compressType: " << compType << " blockType: " << iType << " cipherKey: " << cipherKey.c_str() << " normalize: " << normalize << endl;
//	exit(-3);
#endif


	if (!append) {	// == 0 then create module
	// Try to initialize a default set of datafiles and indicies at our
	// datapath location passed to us from the user.
		if ( compressor ) {
			if ( zText::createModule(path, iType) ) {
				fprintf(stderr, "error: %s: couldn't create module at path: %s \n", program, path);
				exit(-3);
			}
		}
		else if (RawText::createModule(path)) {
			fprintf(stderr, "error: %s: couldn't create module at path: %s \n", program, path);
			exit(-3);
		}
	}

	// Let's see if we can open our input file
	ifstream infile(osisDoc);
	if (infile.fail()) {
		fprintf(stderr, "error: %s: couldn't open input file: %s \n", program, osisDoc);
		exit(-2);
	}

	// Do some initialization stuff
	if (compressor) {
		module = new zText(path, 0, 0, iType, compressor);
	}
	else{
		module = new RawText(path);	// open our datapath with our RawText driver.
	}

	SWFilter *cipherFilter = 0;

	if (!cipherKey.empty()) {
		fprintf(stderr, "Adding cipher filter with phrase: %s\n", cipherKey.c_str() );
		cipherFilter = new CipherFilter(cipherKey.c_str());
		module->AddRawFilter(cipherFilter);
	}

	if (!module->isWritable()) {
		fprintf(stderr, "The module is not writable. Writing text to it will not work.\nExiting.\n" );
		exit(-1);
	}

	activeOsisID[0] = '\0';

	strcpy(currentOsisID,"N/A");

	currentVerse.AutoNormalize(0);
	currentVerse.Headings(1);	// turn on mod/testmnt/book/chap headings
	currentVerse.Persist(1);


	module->setKey(currentVerse);
	module->setPosition(TOP);

	SWBuf token;
	SWBuf text;
	bool intoken = false;
	bool inWhitespace = false;
	bool seeingSpace = false;
	char curChar = '\0';

	while (infile.good()) {
		
		curChar = infile.get();

		// skip the character if it is bad. infile.good() will catch the problem
		if (curChar == -1) {
			continue;
		}

		if (!intoken && curChar == '<') {
			intoken = true;
			token = "<";
			continue;
		}

		// Outside of tokens merge adjacent whitespace
		if (!intoken) {
			seeingSpace = isspace(curChar);
			if (seeingSpace) {
				if (inWhitespace) {
					continue;
				}
				// convert all whitespace to blanks
				curChar = ' ';
			}
			inWhitespace = seeingSpace;
		}

		if (intoken && curChar == '>') {
			intoken = false;
			inWhitespace = false;
			token.append('>');
			// take this isalpha if out to check for bugs in text
			if ((isalpha(token[1])) || (isalpha(token[2]))) {
				//cout << "Handle:" << token.c_str() << endl;
				XMLTag t = transformBSP(token.c_str());

				if (!handleToken(text, t)) {
					text.append(t);
				}
			}
			continue;
		}

		if (intoken) {
			token.append(curChar);
		}
		else {
			switch (curChar) {
				case '>' : text.append("&gt;"); break;
				case '<' : text.append("&lt;"); break;
				default  : text.append(curChar); break;
			}
		}
	}

	// Force the last entry from the text buffer.
	text = "";
	writeEntry(text, true);
	writeLinks();

	delete module;
	if (cipherFilter)
		delete cipherFilter;
	infile.close();

#ifdef _ICU_
	if (converted)  fprintf(stderr, "osis2mod converted %d verses to UTF-8\n", converted);
	if (normalized) fprintf(stderr, "osis2mod normalized %d verses to NFC\n", normalized);
#endif
}

