#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string>

#include <utilstr.h>
#include <filemgr.h>
#include <swmgr.h>
#include <rawtext.h>
#include <iostream>
#include <swbuf.h>
#include <utilxml.h>
#include <listkey.h>
#include <versekey.h>

#include <ztext.h>
// #include <zld.h>
// #include <zcom.h>
#include <lzsscomprs.h>
#include <zipcomprs.h>
// #include <stdio.h>
#include <cipherfil.h>


#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

using namespace std;

SWText *module;
VerseKey *currentVerse = 0;


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
//		std::cerr << (const char*)vk << " == "  << (const char*)test << std::endl;
		return (vk == test);
	}
	else return true;	// no check if we're a heading... Probably bad.
}


void makeKJVRef(VerseKey &key) {
	cout << "re-versified " << key;
//        cout << "\tC" << (int)(key.builtin_books[key.Testament()-1][key.Book()-1].chapmax) << ":V" << (int)(key.builtin_books[key.Testament()-1][key.Book()-1].versemax[key.Chapter()-1]);
       	if (key.Chapter() > key.builtin_books[key.Testament()-1][key.Book()-1].chapmax) {
		key.Chapter(key.builtin_books[key.Testament()-1][key.Book()-1].chapmax);
		key.Verse(key.builtin_books[key.Testament()-1][key.Book()-1].versemax[key.Chapter()-1]);
        }
	else if (key.Verse() > key.builtin_books[key.Testament()-1][key.Book()-1].versemax[key.Chapter()-1]) {
		key.Verse(key.builtin_books[key.Testament()-1][key.Book()-1].versemax[key.Chapter()-1]);
        }
       	cout << "\tas " << key << endl;
}


void writeEntry(VerseKey &key, SWBuf &text, bool suppressOutput = false) {
//	cout << "Verse: " << key << "\n";
//	cout << "TEXT: " << text << "\n\n";
	VerseKey saveKey;
	saveKey.AutoNormalize(0);
	saveKey.Headings(1);
	saveKey = key;

	if (!isKJVRef(key)) {
		makeKJVRef(key);
	}

	SWBuf currentText = module->getRawEntry();
	if (currentText.length()) {
		if (!suppressOutput) {
			cout << "Overwriting entry: " << key << endl;
		}
		text = currentText + " " + text;
	}
	module->setEntry(text);

	key = saveKey;
}


void linkToEntry(VerseKey& dest) {
//	cout << "Verse: " << key << "\n";
//	cout << "TEXT: " << text << "\n\n";
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


bool handleToken(SWBuf &text, XMLTag token) {
	static bool inHeader = false;
	static bool inVerse = true;
	static SWBuf headerType = "";
	static SWBuf header = "";
	static SWBuf lastTitle = "";
	static int titleOffset = -1;
	static ListKey lastVerseIDs = ListKey();

	if ((!strcmp(token.getName(), "title")) && !token.isEndTag() && !token.getAttribute("eID")) {
		titleOffset = text.length(); //start of the title tag
		lastTitle = "";
		return false; 
	}
	else if ((!strcmp(token.getName(), "title")) && (token.isEndTag() || (token.getAttribute("eID")))) {
		lastTitle.append(text.c_str() + titleOffset); //<title ...> up to the end </title>
 		lastTitle.append(token); //</title>
		
// 		printf("lastTitle:	%s\n", lastTitle.c_str());b
// 		printf("text-lastTitle:	%s\n", text.c_str()+titleOffset);
		return false; //don't add </title> to the text itself
	}



//-- START TAG WITH OSIS ID -------------------------------------------------------------------------

	if ((!token.isEndTag()) && (!token.getAttribute("eID")) && (token.getAttribute("osisID"))) {


		// BOOK START
		if ((!strcmp(token.getName(), "div")) && (!strcmp(token.getAttribute("type"), "book"))) {
			inVerse = false;
			if (inHeader) {	// this one should never happen, but just in case
	//			cout << "HEADING ";
				currentVerse->Testament(0);
				currentVerse->Book(0);
				currentVerse->Chapter(0);
				currentVerse->Verse(0);
				writeEntry(*currentVerse, text);
				inHeader = false;
			}
			*currentVerse = token.getAttribute("osisID");
			currentVerse->Chapter(0);
			currentVerse->Verse(0);
			inHeader = true;
			headerType = "book";
			lastTitle = "";
			text = "";

			return true;
		}

		// CHAPTER START
		else if (((!strcmp(token.getName(), "div")) && (!strcmp(token.getAttribute("type"), "chapter")))
				|| (!strcmp(token.getName(), "chapter"))
				) {
			inVerse = false;
			if (inHeader) {
	//			cout << "HEADING ";
				writeEntry(*currentVerse, text);
				inHeader = false;
			}

			*currentVerse = token.getAttribute("osisID");
			currentVerse->Verse(0);
			inHeader = true;
			headerType = "chap";
			lastTitle = "";
			text = "";

			return true;
		}

		// VERSE START
		else if (!strcmp(token.getName(), "verse")) {
			inVerse = true;
			if (inHeader) {
				//make sure we don't insert the preverse title which belongs to the first verse of this chapter!
				const char* chapterTagEnd = strchr(text.c_str(), '>');
				const char* titleTagStart = strstr(text.c_str(), "<title");

				if (chapterTagEnd+1 == titleTagStart) {
					const char* titleTagEnd = strstr(text.c_str(), "</title>");
					while (strstr(titleTagEnd+8, "</title>")) {
						titleTagEnd = strstr(titleTagEnd+8, "</title>");
					}

					
					const char* textEnd = text.c_str() + text.length();
					if (titleTagEnd+8 == textEnd) {
						text.setSize(chapterTagEnd+1-text.c_str()); //only insert the <chapter...> part
					}
				}
				
	// 			cout << "HEADING "<< text.c_str() << endl;
				writeEntry(*currentVerse, text);
			
				text = "";
				inHeader = false;
			}

			SWBuf keyVal = token.getAttribute("osisID");
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
			if (lastVerseIDs.Count())
				*currentVerse = lastVerseIDs.getElement(0)->getText();

			return true;
		}
	}


//-- END TAG ---------------------------------------------------------------------------------------------

	else if ((token.isEndTag()) || (token.getAttribute("eID"))) {

		// VERSE END
		if (!strcmp(token.getName(), "verse")) {
			inVerse = false;
			if (lastTitle.length()) {
				const char* end = strchr(lastTitle, '>');
	// 			printf("length=%d, tag; %s\n", end+1 - lastTitle.c_str(), lastTitle.c_str());

				SWBuf titleTagText;
				titleTagText.append(lastTitle.c_str(), end+1 - lastTitle.c_str());
	// 			printf("tagText: %s\n", titleTagText.c_str());

				XMLTag titleTag(titleTagText);
				titleTag.setAttribute("type", "section");
				titleTag.setAttribute("subtype", "x-preverse");

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
			return true;
		}
	
		// OTHER MISC END TAGS WHEN !INVERSE
		// we really should decide how to handle end tags /e.g. of a chapter). There's no way for frontends to
		// see to what OSIS tag the end tag (which is added to the verse text!) belongs. It mixes up the rendering as a result 
		// included /div for now (jansorg)
		else if (!inVerse &&
				(!strcmp(token.getName(), "p") ||
				 !strcmp(token.getName(), "div") ||
				 !strcmp(token.getName(), "q")  ||
				 !strcmp(token.getName(), "l") ||
				 !strcmp(token.getName(), "lg"))) {
     

			text.append(token);
			writeEntry(*currentVerse, text, true);
			text = "";

			return true;

		}
	}
	return false;
}

int main(int argc, char **argv) {

	// Let's test our command line arguments
	if (argc < 3) {
		fprintf(stderr, 
"\nusage: osis2mod <output/path> <osisDoc> [createMod] [compressType [blockType [cipherKey]]]\n");
		fprintf(stderr, "  createMod   : (default 0): 0 - create  1 - augment\n");
		fprintf(stderr, "  compressType: (default 0): 0 - no compression  1 - LZSS    2 - Zip\n");
		fprintf(stderr, "  blockType   : (default 4): 2 - verses  3 - chapters  4 - books\n");
		exit(-1);
	}

	int iType = 4;
	int compType = 0;
	string cipherKey = "";
	SWCompress *compressor = 0;
// 	SWModule *outModule    = 0;
	

	if (argc > 4) {
		compType = atoi(argv[4]);
		if (argc > 5) {
			iType = atoi(argv[5]);
			if (argc > 6) {
				cipherKey = argv[6];
			}
		}
	}

	switch (compType) {	// these are deleted by zText
		case 0: break;
		case 1: compressor = new LZSSCompress(); break;
		case 2: compressor = new ZipCompress(); break;
	}

// 	cerr << "path: " << argv[1] << " osisDoc: " << argv[2] << " create: " << argv[3] << " compressType: " << compType << " blockType: " << iType << " cipherKey: " << cipherKey.c_str() << "\n";
// 	cerr << "";
// 	exit(-3);


	if ((argc<4)||(!strcmp(argv[3], "0"))) {	// == 0 then create module
	// Try to initialize a default set of datafiles and indicies at our
	// datapath location passed to us from the user.
		if ( compressor ){
			if ( zText::createModule(argv[1], iType) ){
				fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[1]);
				exit(-3);
			}
		}
		else if (RawText::createModule(argv[1])) {
			fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[1]);
			exit(-3);
		}
	}

	// Let's see if we can open our input file
	FileDesc *fd = FileMgr::getSystemFileMgr()->open(argv[2], FileMgr::RDONLY);
	if (fd->getFd() < 0) {
		fprintf(stderr, "error: %s: couldn't open input file: %s \n", argv[0], argv[2]);
		exit(-2);
	}

	// Do some initialization stuff
	SWBuf buffer;

	if (compressor){
		module = new zText(argv[1], 0, 0, iType, compressor);
	}
	else{
		module = new RawText(argv[1]);	// open our datapath with our RawText driver.
	}

	if (!cipherKey.empty()){
		SWFilter *cipherFilter = new CipherFilter(cipherKey.c_str());
		module->AddRawFilter(cipherFilter);
	}

	currentVerse = new VerseKey();
	currentVerse->AutoNormalize(0);
	currentVerse->Headings(1);	// turn on mod/testmnt/book/chap headings
	currentVerse->Persist(1);

	module->setKey(*currentVerse);

	(*module) = TOP;

	const char *from;
	SWBuf token;
	SWBuf text;
	bool intoken = false;

	while (FileMgr::getLine(fd, buffer)) {
		for (from = buffer.c_str(); *from; from++) {
			if (*from == '<') {
				intoken = true;
				token = "<";
				continue;
			}

			if (*from == '>') {
				intoken = false;
				token.append('>');
				// take this isalpha if out to check for bugs in text
				if ((isalpha(token[1])) || (isalpha(token[2]))) {
					if (!handleToken(text, token.c_str())) {
						text.append(token);
					}
				}
				continue;
			}

			if (intoken)
				token.append(*from);
			else	
				text.append(*from);
		}
	}
	delete module;
	delete currentVerse;
	FileMgr::getSystemFileMgr()->close(fd);
}

