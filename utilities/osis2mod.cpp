#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <swmgr.h>
#include <rawtext.h>
#include <iostream>
#include <swbuf.h>
#include <utilxml.h>
#include <listkey.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

using namespace std;

RawText *module;
VerseKey *currentVerse = 0;

char readline(int fd, char **buf) {
	char ch;
	if (*buf)
		delete [] *buf;
	*buf = 0;
	int len;


	long index = lseek(fd, 0, SEEK_CUR);
	// clean up any preceding white space
	while ((len = read(fd, &ch, 1)) == 1) {
		if ((ch != 13) && (ch != ' ') && (ch != '\t'))
			break;
		else index++;
	}


	while (ch != 10) {
        if ((len = read(fd, &ch, 1)) != 1)
			break;
	}
	
	int size = (lseek(fd, 0, SEEK_CUR) - index) - 1;

	*buf = new char [ size + 1 ];

	if (size > 0) {
		lseek(fd, index, SEEK_SET);
		read(fd, *buf, size);
		read(fd, &ch, 1);   //pop terminating char
		(*buf)[size] = 0;

		// clean up any trailing junk on buf
		for (char *it = *buf+(strlen(*buf)-1); it > *buf; it--) {
			if ((*it != 10) && (*it != 13) && (*it != ' ') && (*it != '\t'))
				break;
			else *it = 0;
		}
	}
	else **buf = 0;
	return !len;
}

char* deleteSubverses(char *buf) {
        // remove subverse elements from osisIDs
        // (this is a hack and should be handled better with VerseKey2)
        int dots = 0;
        for (int i = 0; buf[i]; i++) {
               	if (buf[i] == ' ') {
               		dots = 0;
                }
                else if (buf[i] == '.') {
                      	dots++;
                        if (dots > 2) {
                               	while (buf[i] && buf[i] != ' ') {
                                        buf[i] = ' ';
                               		i++;
                                }
                                i--;
                                dots = 0;
                        }
                }
        }
        return buf;
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

void writeEntry(VerseKey &key, SWBuf &text) {
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
        	cout << "Overwriting entry: " << key << endl;
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

        makeKJVRef(*currentVerse);

	cout << "Linking " << module->KeyText() << " to " << dest.getText() << "\n";
	module->linkEntry(&dest);

	*currentVerse = saveKey;
}


bool handleToken(SWBuf &text, XMLTag token) {
	static bool inHeader = false;
	static SWBuf headerType = "";
	static SWBuf header = "";
	static SWBuf lastTitle = "";
	static int titleOffset = -1;
	static ListKey lastVerseIDs = ListKey();

	if ((!strcmp(token.getName(), "title")) && (!token.isEndTag())) {
		titleOffset = text.length();
		return false;
	}
	else if ((!strcmp(token.getName(), "title")) && (token.isEndTag())) {
		lastTitle = (text.c_str() + titleOffset);
		lastTitle += token;
		return false;
	}
	else if (((!strcmp(token.getName(), "div")) && (!token.isEndTag()) && (token.getAttribute("osisID"))) && (!strcmp(token.getAttribute("type"), "book"))) {
		if (inHeader) {	// this one should never happen, but just in case
//			cout << "HEADING ";
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
	}
	else if ((((!strcmp(token.getName(), "div")) && (!token.isEndTag()) && (token.getAttribute("osisID"))) && (!strcmp(token.getAttribute("type"), "chapter"))) || ((!strcmp(token.getName(), "chapter")) && (!token.isEndTag()) && (token.getAttribute("osisID")))) {
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
	}
	else if ((!strcmp(token.getName(), "verse")) && (!token.isEndTag())) {
		if (inHeader) {
//			cout << "HEADING ";
			writeEntry(*currentVerse, text);
			inHeader = false;
		}

		string str = token.getAttribute("osisID");
                char *subverseBuf = new char[str.length()+1];
                strcpy(subverseBuf, str.c_str());
		str = deleteSubverses(subverseBuf);
                delete subverseBuf;

		*currentVerse = str.c_str();

		int pos = 0;
		while ((pos = str.find(' ', pos)) != string::npos) {
			str.replace(pos, 1, ";");
		}

		//cout << "set the list\n" << token.getAttribute("osisID");
		lastVerseIDs = currentVerse->ParseVerseList(str.c_str());
//		if (lastVerseIDs.Count() > 1)
//			cout << "count is" << lastVerseIDs.Count();

		if (lastVerseIDs.Count())
			*currentVerse = lastVerseIDs.getElement(0)->getText();

		text = token;

		return true;
	}
	else if ((!strcmp(token.getName(), "verse")) && (token.isEndTag())) {
		if (lastTitle.length()) {
			SWBuf titleHead = lastTitle;
			char *end = strchr(lastTitle.getRawData(), '>');
			titleHead.setSize((end - lastTitle.getRawData())+1);
			XMLTag titleTag(titleHead);
			titleTag.setAttribute("type", "section");
			titleTag.setAttribute("subtype", "x-preverse");
			text = SWBuf(titleTag) + SWBuf(end+1) + text;
		}
		text += token;
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
	return false;
}




int main(int argc, char **argv) {

	// Let's test our command line arguments
	if (argc < 3) {
		fprintf(stderr, "usage: %s <path/to/mod/files> <osisDoc> [0|1 - create module|augment]\n\n", argv[0]);
		exit(-1);
	}


	if ((argc<4)||(!strcmp(argv[3], "0"))) {	// == 0 then create module
	// Try to initialize a default set of datafiles and indicies at our
	// datapath location passed to us from the user.
		if (RawText::createModule(argv[1])) {
			fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[1]);
			exit(-3);
		}
	}

	// Let's see if we can open our input file
	int fd = open(argv[2], O_RDONLY|O_BINARY);
	if (fd < 0) {
		fprintf(stderr, "error: %s: couldn't open input file: %s \n", argv[0], argv[2]);
		exit(-2);
	}

	// Do some initialization stuff
	char *buffer = 0;
	module = new RawText(argv[1]);	// open our datapath with our RawText driver.
	currentVerse = new VerseKey();
	currentVerse->AutoNormalize(0);
	currentVerse->Headings(1);	// turn on mod/testmnt/book/chap headings
	currentVerse->Persist(1);

	module->setKey(*currentVerse);

	(*module) = TOP;

	char *from;
	SWBuf token;
	SWBuf text;
	bool intoken = false;

	while (!readline(fd, &buffer)) {
		for (from = buffer; *from; from++) {
			if (*from == '<') {
				intoken = true;
				token = "<";
				continue;
			}

			if (*from == '>') {
				intoken = false;
				token += ">";
				// take this isalpha if out to check for bugs in text
				if ((isalpha(token[1])) || (isalpha(token[2]))) {
					if (!handleToken(text, token.c_str())) {
						text += token;
					}
				}
				continue;
			}

			if (intoken)
				token += *from;
			else	text += *from;
		}
	}
	// clear up our buffer that readline might have allocated
	if (buffer)
		delete [] buffer;
	delete module;
	delete currentVerse;
	close(fd);
}

