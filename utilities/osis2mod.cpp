#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <stack>
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
// #include <zld.h>
// #include <zcom.h>
#include <lzsscomprs.h>
#include <zipcomprs.h>
#include <cipherfil.h>

#ifdef _ICU_
#include <utf8nfc.h>
#endif

//#define DEBUG

// Debug for simple transformation stack
//#define DEBUG2

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

using namespace std;

#ifdef _ICU_
UTF8NFC normalizer;
#endif

SWText *module = 0;
VerseKey *currentVerse = 0;
char activeOsisID[255];
char currentOsisID[255];
char *osisabbrevs[] = {"Gen", "Exod", "Lev", "Num", "Deut", "Josh", "Judg",
	"Ruth", "1Sam", "2Sam", "1Kgs", "2Kgs", "1Chr", "2Chr", "Ezra", "Neh",
	"Esth", "Job", "Ps", "Prov", "Eccl", "Song", "Isa", "Jer", "Lam", "Ezek",
	"Dan", "Hos", "Joel", "Amos", "Obad", "Jonah", "Mic", "Nah", "Hab",
	"Zeph", "Hag", "Zech", "Mal",

	"Matt", "Mark", "Luke", "John", "Acts", "Rom", "1Cor", "2Cor", "Gal",
	"Eph", "Phil", "Col", "1Thess", "2Thess", "1Tim", "2Tim", "Titus",
	"Phlm", "Heb", "Jas", "1Pet", "2Pet", "1John", "2John", "3John",
	"Jude", "Rev"};

static bool inCanonicalOSISBook = true; // osisID is for a book that is not in Sword's canon
static bool normalize = false; // Whether to normalize UTF-8 to NFC

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

// remove subverse elements from osisIDs
void deleteSubverses(SWBuf &buf) {
	for (int i = 0; buf[i]; i++) {
		if (buf[i] == '!') {
			while (buf[i] && buf[i] != ' ') {
				buf[i] = ' ';
				i++;
			}
			i--;
		}
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


void makeKJVRef(VerseKey &key) {
	cout << "re-versified " << key;
#ifdef DEBUG
	cout << "\tC" << (int)(key.builtin_books[key.Testament()-1][key.Book()-1].chapmax) << ":V" << (int)(key.builtin_books[key.Testament()-1][key.Book()-1].versemax[key.Chapter()-1]);
#endif
	if (key.Chapter() > key.builtin_books[key.Testament()-1][key.Book()-1].chapmax) {
		key.Chapter(key.builtin_books[key.Testament()-1][key.Book()-1].chapmax);
		key.Verse(key.builtin_books[key.Testament()-1][key.Book()-1].versemax[key.Chapter()-1]);
	}
	else if (key.Verse() > key.builtin_books[key.Testament()-1][key.Book()-1].versemax[key.Chapter()-1]) {
		key.Verse(key.builtin_books[key.Testament()-1][key.Book()-1].versemax[key.Chapter()-1]);
	}
	cout << "\tas " << key << endl;
}


void writeEntry(VerseKey &key, SWBuf &text, bool force = false) {
	static SWBuf activeVerseText;
	char keyOsisID[255];

	if (inCanonicalOSISBook) {
		strcpy(keyOsisID, key.getOSISRef());

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
		saveKey = key;

		// If we have seen a verse and the supplied one is different then we output the collected one.
		if (*activeOsisID && strcmp(activeOsisID, keyOsisID)) {

			key = lastKey;

			if (!isKJVRef(key)) {
				makeKJVRef(key);
			}

#ifdef _ICU_
			if (normalize) {
				normalizer.processText(activeVerseText, (SWKey *)2);  // note the hack of 2 to mimic a real key. TODO: remove all hacks
			}
#endif

			SWBuf currentText = module->getRawEntry();
			if (currentText.length()) {
				cout << "Appending entry: " << key.getOSISRef() << ": " << activeVerseText << endl;
				activeVerseText = currentText + " " + activeVerseText;
			}

#ifdef DEBUG
			cout << "Write: " << activeOsisID << ":" << key.getOSISRef() << ": " << activeVerseText << endl;
#endif

			module->setEntry(activeVerseText);
			activeVerseText = "";
		}

		// eliminate leading whitespace on the beginning of each verse and
		// before we append to current content, since we just added one
		text.trimStart();
		if (activeVerseText.length()) {
			activeVerseText += " ";
			activeVerseText += text;
		}
		else {
			activeVerseText = text;
		}

		key = saveKey;
		lastKey = key;
		strcpy(activeOsisID, keyOsisID);
	}
}


void linkToEntry(VerseKey& dest) {
	//cout << "Verse: " << key << "\n";
	//cout << "TEXT: " << text << "\n\n";
	//SWBuf currentText = module->getRawEntry();
	//if (currentText.length())
	//	text = currentText + " " + text;
	VerseKey saveKey;
	saveKey.AutoNormalize(0);
	saveKey.Headings(1);
	saveKey = *currentVerse;

	if (!isKJVRef(*currentVerse)) {
		makeKJVRef(*currentVerse);
	}

	cout << "Linking " << module->KeyText() << " to " << dest.getText() << "\n";
	module->linkEntry(&dest);

	*currentVerse = saveKey;
}

// Return true if the content was handled or is to be ignored.
//		false if the what has been seen is to be accumulated and considered later.
bool handleToken(SWBuf &text, XMLTag *token) {

	// Everything between the begin book tag and the first begin chapter tag is inBookHeader
	static bool inBookHeader = false;
	// Everything between the begin chapter tag and the first begin verse tag is inChapterHeader
	static bool inChapterHeader = false;

	// Flags to indicate whether we are in a book, chapter and/or verse
	//static bool inBook = false;
	//static bool inChapter = false;
	static bool inVerse = true;

	static SWBuf header = "";

	// Used to remember titles that need to be handle specially
	static SWBuf lastTitle = "";
	static int titleOffset = -1;
	static bool inTitle = false;
	static int titleDepth = 0;

	static ListKey lastVerseIDs = ListKey();

	// Stack of elements used to validate that books, chapters and verses are well-formed
	// This goes beyond simple xml well-formed and also considers milestoned div, chapter and verse
	// to be begin and end tags, too.
	// It is an error if books and chapters are not well formed (though not required by OSIS)
	// It is a warning that verses are not well formed (because some clients are not ready)
	static std::stack<XMLTag*> tagStack;
	// The following are used to validate well-formedness
	static int chapterDepth = 0;
	static int bookDepth = 0;
	static int verseDepth = 0;

	int tagDepth = tagStack.size();
	const char *tokenName = token->getName();
	bool isEndTag = token->isEndTag() || token->getAttribute("eID");
	const char *typeAttr = token->getAttribute("type");

	//Titles are treated specially.
	// If the title has an attribute type of "main" or "chapter"
	// it belongs to its <div> or <chapter> and is treated as part of its heading
	// Otherwise if it a title in a chapter before the first the first verse it
	// is put into the verse as a preverse title.
	if (!token->isEmpty() && !isEndTag && titleDepth == 0 && (!strcmp(tokenName, "title")) && (!typeAttr || (strcmp(typeAttr, "main") && strcmp(typeAttr, "chapter")))) {
		titleOffset = text.length(); //start of the title tag
		lastTitle = "";
		inTitle = true;
		tagStack.push(token);
#ifdef DEBUG
		cout << currentOsisID << ": push (" << tagStack.size() << ") " << token->getName() << endl;
#endif
		titleDepth = tagStack.size();
		return false;
	}
	// Check titleDepth since titles can be nested. Don't want to quit too early.
	else if (isEndTag && tagDepth == titleDepth && (!strcmp(tokenName, "title"))) {
		lastTitle.append(text.c_str() + titleOffset); //<title ...> up to the end </title>
		lastTitle.append(*token); //</title>

#ifdef DEBUG
		cout << currentOsisID << ":" << endl;
 		cout << "\tlastTitle:      " << lastTitle.c_str() << endl;
 		cout << "\ttext-lastTitle: " << text.c_str()+titleOffset << endl;
		cout << "\ttext:	   " << text.c_str() << endl;
#endif
		inTitle = false;
		titleDepth = 0;
#ifdef DEBUG
		cout << currentOsisID << ": pop(" << tagStack.size() << ") " << tagStack.top()->getName() << endl;
#endif
		tagStack.pop();
		return false; // don't add </title> to the text itself
	}



//-- START TAG -------------------------------------------------------------------------

	if (!isEndTag) {

		// Remember non-empty start tags
		if (!token->isEmpty()) {
			tagStack.push(token);
#ifdef DEBUG
			cout << currentOsisID << ": push (" << tagStack.size() << ") " << token->getName() << endl;
#endif
		}

		//-- WITH OSIS ID -------------------------------------------------------------------------
		if (token->getAttribute("osisID")) {

			// BOOK START
			if ((!strcmp(tokenName, "div")) && (typeAttr && !strcmp(typeAttr, "book"))) {
				inVerse = false;
				if (inBookHeader || inChapterHeader) {	// this one should never happen, but just in case
#ifdef DEBUG
					cout << currentOsisID << ": HEADING ";
#endif
					currentVerse->Testament(0);
					currentVerse->Book(0);
					currentVerse->Chapter(0);
					currentVerse->Verse(0);
					writeEntry(*currentVerse, text);
				}
				strcpy(currentOsisID, token->getAttribute("osisID"));
				*currentVerse = currentOsisID;
				currentVerse->Chapter(0);
				currentVerse->Verse(0);
				inBookHeader = true;
				inChapterHeader = false;
				lastTitle = "";
				text = "";
				bookDepth = tagStack.size();
				chapterDepth = 0;
				verseDepth = 0;

				inCanonicalOSISBook = isOSISAbbrev(token->getAttribute("osisID"));

				return true;
			}

			// CHAPTER START
			else if (((!strcmp(tokenName, "div")) && (typeAttr && !strcmp(typeAttr, "chapter")))
					 || (!strcmp(tokenName, "chapter"))
					 ) {
				inVerse = false;
				if (inBookHeader) {
#ifdef DEBUG
					cout << currentOsisID << ": BOOK HEADING "<< text.c_str() << endl;
#endif
					writeEntry(*currentVerse, text);
				}

				strcpy(currentOsisID, token->getAttribute("osisID"));
				*currentVerse = currentOsisID;
				currentVerse->Verse(0);
				inBookHeader = false;
				inChapterHeader = true;
				lastTitle = "";
				text = "";
				chapterDepth = tagStack.size();
				verseDepth = 0;

				return true;
			}

			// VERSE START
			else if (!strcmp(tokenName, "verse")) {
				inVerse = true;
				if (inChapterHeader) {
					SWBuf heading = text;

					//make sure we don't insert the preverse title which belongs to the first verse of this chapter!
					// Did we have a preverse title?
					if (lastTitle.length())
					{
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
#ifdef DEBUG
						cout << currentOsisID << ": CHAPTER HEADING "<< heading.c_str() << endl;
#endif
						writeEntry(*currentVerse, heading);
					}

					inChapterHeader = false;
				}

				SWBuf keyVal = token->getAttribute("osisID");
				deleteSubverses(keyVal);

				// turn "Mat.1.1  Mat.1.2" into "Mat.1.1; Mat.1.2"
				bool skipSpace = false;
				for (int i = 0; keyVal[i]; i++) {
					if (keyVal[i] == ' ') {
						if (!skipSpace) {
							keyVal[i] = ';';
							skipSpace = true;
						}
					}
					else skipSpace = false;
				}

				lastVerseIDs = currentVerse->ParseVerseList(keyVal);
				if (lastVerseIDs.Count() > 1) {
					*currentVerse = lastVerseIDs.getElement(0)->getText();
					strcpy(currentOsisID, currentVerse->getOSISRef());
				}
				else {
					strcpy(currentOsisID, keyVal.c_str());
					*currentVerse = currentOsisID;
				}

				verseDepth = tagStack.size();

				return true;
			}
		}
		// Handle stuff between the verses
		// Whitespace producing empty tokens are appended to prior entry
		// Also the quote
		// This is a hack to get ESV to work
		else if (!inTitle && !inVerse && token->isEmpty()) { // && !inBookHeader && !inChapterHeader) {
			if (!strcmp(tokenName, "p") ||
					!strcmp(tokenName, "div") ||
					!strcmp(tokenName, "q")  ||
					!strcmp(tokenName, "l") ||
					!strcmp(tokenName, "lb") ||
					!strcmp(tokenName, "lg")
					) {
#ifdef DEBUG
					if (token) {
						cout << currentOsisID << ": appending interverse start token " << *token << ":" << text.c_str() << endl;
					}
#endif
				SWBuf tmp = token->toString();
				writeEntry(*currentVerse, tmp);
				return true;
			}
#ifdef DEBUG
			else {
				if (token) {
					cout << currentOsisID << ": interverse start token " << *token << ":" << text.c_str() << endl;
				}
			}
#endif
		}
	}

//-- EMPTY and END TAG ---------------------------------------------------------------------------------------------

	else {

		if (tagStack.empty()) {
			cout << currentOsisID << ": tag expected" << endl;
			exit(1);
		}

		XMLTag* topToken = 0;
		if (!token->isEmpty()) {
			topToken = tagStack.top();
			tagDepth = tagStack.size();
#ifdef DEBUG
			cout << currentOsisID << ": pop(" << tagDepth << ") " << topToken->getName() << endl;
#endif
			tagStack.pop();

			if (strcmp(topToken->getName(), tokenName)) {
				cout << "Error: " << currentOsisID << ": Expected " << topToken->getName() << " found " << tokenName << endl;
//				exit(1);	// I'm sure this validity check is a good idea, but there's a but somewhere that's killing the converter here.
						// So I'm disabling this line. Unvalidated OSIS files shouldn't be run through the converter anyway.
			}
		}

		// VERSE END
		if (!strcmp(tokenName, "verse")) {
			inVerse = false;

			if (tagDepth != verseDepth) {
				cout << "Warning verse " << currentOsisID << " is not well formed:(" << verseDepth << "," << tagDepth << ")" << endl;
			}

			if (lastTitle.length()) {
				const char* end = strchr(lastTitle, '>');
#ifdef DEBUG
				cout << currentOsisID << ":" << endl;
				cout << "\t" << lastTitle << endl;
	 			cout << "\tlength=" << int(end+1 - lastTitle.c_str()) << ", tag:" << lastTitle.c_str() << endl;
#endif

				SWBuf titleTagText;
				titleTagText.append(lastTitle.c_str(), end+1 - lastTitle.c_str());
#ifdef DEBUG
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
	//		text += token;
			writeEntry(*currentVerse, text);

			// If we found an osisID like osisID="Gen.1.1 Gen.1.2 Gen.1.3" we have to link Gen.1.2 and Gen.1.3 to Gen.1.1
			VerseKey dest = *currentVerse;
			for (int i = 0; i < lastVerseIDs.Count(); ++i) {
				VerseKey linkKey;
				linkKey.AutoNormalize(0);
				linkKey.Headings(1);	// turn on mod/testmnt/book/chap headings
				linkKey.Persist(1);
				linkKey = lastVerseIDs.getElement(i)->getText();

				if (linkKey.Verse() != currentVerse->Verse() || linkKey.Chapter() != currentVerse->Chapter() || linkKey.Book() != currentVerse->Book() || linkKey.Testament() != currentVerse->Testament()) {
					*currentVerse = linkKey;
					linkToEntry(dest);
				}
			}

			lastTitle = "";
			text = "";
			verseDepth = 0;
			return true;
		}
		else if (!inTitle && !inVerse && !inBookHeader && !inChapterHeader) {
			// Is this the end of a chapter.
			if (tagDepth == chapterDepth && (!strcmp(tokenName, "div") || !strcmp(tokenName, "chapter"))) {
				chapterDepth = 0;
				verseDepth = 0;
				text = "";
				return true;
			}
			// Or is it the end of a book
			else if (tagDepth == bookDepth && (!strcmp(tokenName, "div"))) {
				bookDepth = 0;
				chapterDepth = 0;
				verseDepth = 0;
				text = "";
				return true;
			}
			// Or is it the end of an osis document
			else if (!strcmp(tokenName, "osisText") || !strcmp(tokenName, "osis")) {
				bookDepth = 0;
				chapterDepth = 0;
				verseDepth = 0;
				text = "";
				return true;
			}
			// OTHER MISC END TAGS WHEN !INVERSE
			// Test that is between verses, or after the last is appended to the preceeding verse.
			else if (!strcmp(tokenName, "p") ||
					!strcmp(tokenName, "div") ||
					!strcmp(tokenName, "q")  ||
					!strcmp(tokenName, "l") ||
					!strcmp(tokenName, "lb") ||
					!strcmp(tokenName, "lg")
					) {
				text.append(*token);
				writeEntry(*currentVerse, text);
				text = "";
#ifdef DEBUG
				cout << currentOsisID << ": appending interverse end tag: " << tokenName << "(" << tagDepth << "," << chapterDepth << "," << bookDepth << ")" << endl;
#endif
				return true;
			}
#ifdef DEBUG
			cout << currentOsisID << ": interverse end tag: " << tokenName << "(" << tagDepth << "," << chapterDepth << "," << bookDepth << ")" << endl;
#endif
		}
	}
	return false;
}

XMLTag* transform(XMLTag* t) {
	static std::stack<XMLTag*> tagStack;
	static int sID = 1;
	char buf[11];

	// Support simplification transformations
	if (!t->isEmpty()) {
		if (!t->isEndTag()) {
			tagStack.push(t);
#ifdef DEBUG2
			cout << currentOsisID << ": xform push (" << tagStack.size() << ") " << t->getName() << endl;
#endif
			// Transform <q> into <q sID=""/> except for <q who="Jesus">
			if ((!strcmp(t->getName(), "q")) && (!t->getAttribute("who") || strcmp(t->getAttribute("who"), "Jesus"))) {
				t->setEmpty(true);
				sprintf(buf, "q%d", sID++);
				t->setAttribute("sID", buf);
			}

			// Transform <p> into <lb type="x-begin-paragraph"/>
			else if (!strcmp(t->getName(), "p")) {
				// note there is no process that should care about type, it is there for reversability
				t->setText("<lb type=\"x-begin-paragraph\" />");
			}
		}
		else {
			XMLTag *topToken = tagStack.top();
#ifdef DEBUG2
			cout << currentOsisID << ": xform pop(" << tagStack.size() << ") " << topToken->getName() << endl;
#endif
			tagStack.pop();

			// If we have found an end tag for a <q> that was transformed then transform this one as well.
			if ((!strcmp(t->getName(), "q")) && (!strcmp(topToken->getName(), "q")) && (!topToken->getAttribute("who") || strcmp(topToken->getAttribute("who"), "Jesus"))) {
				// make this a clone of the start tag with sID changed to eID
				*t = *topToken;
				t->setAttribute("eID", t->getAttribute("sID"));
				t->setAttribute("sID", 0);
			}

			// Look for paragraph tags.
			// If we have found an end tag for a <p> that was transformed then transform this as well.
			else if ((!strcmp(t->getName(), "p")) && (!strcmp(topToken->getName(), "lb"))) {
				t->setText("<lb type=\"x-end-paragraph\" />");
			}
		}
	}
	return t;
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
	fprintf(stderr, "  -n\t\t\t normalize UTF-8 to NFC (default is to leave text unmodified)\n");
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
		else if (!strcmp(argv[i], "-n")) {
			normalize = true;
#ifndef _ICU_
			normalize = false;
			cout << program << " is not compiled with support for ICU. Ignoring -n flag." << endl;
#endif
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

#ifdef DEBUG
	cout << "path: " << path << " osisDoc: " << osisDoc << " create: " << append << " compressType: " << compType << " blockType: " << iType << " cipherKey: " << cipherKey.c_str() << " normalize: " << normalize << "\n";
	cout << "";
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

	currentVerse = new VerseKey();
	currentVerse->AutoNormalize(0);
	currentVerse->Headings(1);	// turn on mod/testmnt/book/chap headings
	currentVerse->Persist(1);

	module->setKey(*currentVerse);

	(*module) = TOP;

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
				XMLTag *t = new XMLTag(token.c_str());

				if (!handleToken(text, transform(t))) {
					text.append(*t);
				}
			}
			continue;
		}

		if (intoken)
			token.append(curChar);
		else
			switch (curChar) {
				case '>' : text.append("&gt;"); break;
				case '<' : text.append("&lt;"); break;
				default  : text.append(curChar); break;
			}
	}

	// Force the last entry from the text buffer.
	text = "";
	writeEntry(*currentVerse, text, true);
	delete module;
	delete currentVerse;
	if (cipherFilter)
		delete cipherFilter;
	infile.close();
}

