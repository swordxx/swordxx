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
        for (int i = 0; buf[i]; i++) {
		if (buf[i] == '!') {
			while (buf[i] && buf[i] != ' ') {
                        	buf[i] = ' ';
                               	i++;
                        }
                        i--;
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
	if (((!strcmp(token.getName(), "div")) && (!token.isEndTag() && !(token.getAttribute("eID"))) && (token.getAttribute("osisID"))) && (!strcmp(token.getAttribute("type"), "book"))) {
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
	}
	else if ((((!strcmp(token.getName(), "div")) && (!token.isEndTag()) && (token.getAttribute("osisID"))) && (!strcmp(token.getAttribute("type"), "chapter"))) || ((!strcmp(token.getName(), "chapter")) && (!token.isEndTag() || (token.getAttribute("eID"))) && (token.getAttribute("osisID")))) {
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
	}
	else if ((!strcmp(token.getName(), "verse")) && (!token.isEndTag() && !(token.getAttribute("eID")))) {
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

		string str = token.getAttribute("osisID");
                char *subverseBuf = new char[str.length()+1];
                strcpy(subverseBuf, str.c_str());
		str = deleteSubverses(subverseBuf);
                delete []subverseBuf;

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

		text.append( token );

		return true;
	}
	else if ((!strcmp(token.getName(), "verse")) && (token.isEndTag() || (token.getAttribute("eID")))) {
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
/*        else if (!inVerse && (token.isEndTag() || (token.getAttribute("eID"))) && (!strcmp(token.getName(), "p") || !strcmp(token.getName(), "div") || !strcmp(token.getName(), "q")  || !strcmp(token.getName(), "l") || !strcmp(token.getName(), "lg"))) {*/
     
//we really should decide how to handle end tags /e.g. of a chapter). There's no way for frontends to
//see to what OSIS tag the end tag (which is added to the verse text!) belongs. It mixes up the rendering as a result 
//excluded /div for now (jansorg)
	else if (!inVerse && (token.isEndTag() || (token.getAttribute("eID"))) && (!strcmp(token.getName(), "p") || !strcmp(token.getName(), "q")  || !strcmp(token.getName(), "l") || !strcmp(token.getName(), "lg"))) {
        	//text.append( token );
		writeEntry(*currentVerse, text, true);
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
				token.append('>');
				// take this isalpha if out to check for bugs in text
				if ((isalpha(token[1])) || (isalpha(token[2]))) {
					if (!handleToken(text, token.c_str())) {
						text.append( token );
					}
				}
				continue;
			}

			if (intoken)
				token.append( *from );
			else	
				text.append( *from );
		}
	}
	// clear up our buffer that readline might have allocated
	if (buffer)
		delete [] buffer;
	delete module;
	delete currentVerse;
	close(fd);
}

