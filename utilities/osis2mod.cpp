#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

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


void writeEntry(VerseKey &key, SWBuf &text) {
//	cout << "Verse: " << key << "\n";
//	cout << "TEXT: " << text << "\n\n";
	SWBuf currentText = module->getRawEntry();
	if (currentText.length())
		text = currentText + " " + text;
	module->setEntry(text);
}


bool handleToken(SWBuf &text, XMLTag token) {
	static bool inHeader = false;
	static SWBuf headerType = "";
	static SWBuf header = "";
	static SWBuf lastTitle = "";
	static int titleOffset = -1;

	if ((tag.getName()) && (!strcmp(token.getName(), "title")) && (!token.isEndTag())) {
		titleOffset = text.length();
		return false;
	}
	if ((tag.getName()) && (!strcmp(token.getName(), "title")) && (token.isEndTag())) {
		lastTitle = (text.c_str() + titleOffset);
		lastTitle += token;
		return false;
	}
	if (((tag.getName()) && (!strcmp(token.getName(), "div")) && (!token.isEndTag()) && (token.getAttribute("osisID"))) && (!strcmp(token.getAttribute("type"), "book"))) {
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
	else if ((((tag.getName()) && (!strcmp(token.getName(), "div")) && (!token.isEndTag()) && (token.getAttribute("osisID"))) && (!strcmp(token.getAttribute("type"), "chapter"))) || ((!strcmp(token.getName(), "chapter")) && (!token.isEndTag()) && (token.getAttribute("osisID")))) {
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
	if ((tag.getName()) && (!strcmp(token.getName(), "verse")) && (!token.isEndTag())) {
		if (inHeader) {
//			cout << "HEADING ";
			writeEntry(*currentVerse, text);
			inHeader = false;
		}

		*currentVerse = token.getAttribute("osisID");
		text = "";
		return true;
	}
	if ((tag.getName()) && (!strcmp(token.getName(), "verse")) && (token.isEndTag())) {
		if (lastTitle.length()) {
			SWBuf titleHead = lastTitle;
			char *end = strchr(lastTitle.getRawData(), '>');
			titleHead.setSize((end - lastTitle.getRawData())+1);
			XMLTag titleTag(titleHead);
			titleTag.setAttribute("type", "section");
			titleTag.setAttribute("subtype", "x-preverse");
			text = SWBuf(titleTag) + SWBuf(end+1) + text;
		}
		writeEntry(*currentVerse, text);
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
	  
	int successive = 0;  //part of hack below

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



		
/*
		string verseText = "";

		// chapter number
		if (!strncmp("$$$ ", buffer, 4)) {
			buffer[7] = 0;
			chapter = atoi(buffer+4);
			continue;
		}
		// header
		if (!strncmp("<TD COLSPAN=4 VALIGN=TOP><FONT SIZE=2><B>", buffer, 41)) {
			char *end = strstr(buffer+41, "</B>");
			*end = 0;
			header = buffer+41;
			continue;
		}
		// verse number
		if (!strncmp("<TD VALIGN=TOP ALIGN=RIGHT WIDTH=12><FONT SIZE=2 COLOR=RED><B><SUP>", buffer, 67)) {
			char *end = strstr(buffer+67, "</SUP>");
			*end = 0;
			verse = atoi(buffer+67);
			continue;
		}
		// Actual verse data
		if (!strncmp("<TD VALIGN=TOP><FONT SIZE=2>", buffer, 28)) {
			char *end = strstr(buffer+28, "</FONT>");
			*end = 0;
		}
		// extra
		else {
			continue;
		}

		verseText = buffer + 28;

		if (header.length()) {
			verseText = "<title type=\"section\" subtype=\"x-preverse\">" + header + "</title>" + verseText;
			header = "";
		}

		string vsbuf = argv[3];
		sprintf(tmpBuf, "%i", chapter);
		vsbuf += ((string)" ") + tmpBuf;
		sprintf(tmpBuf, "%i", verse);
		vsbuf += ((string)":") + tmpBuf;
		vk = vsbuf.c_str();
		if (vk.Error()) {
			std::cerr << "Error parsing key: " << vsbuf << "\n";
			exit(-5);
		}
		string orig = mod.getRawEntry();

		if (!isKJVRef(vsbuf.c_str())) {
			VerseKey origVK = vk;
//	This block is functioning improperly -- problem with AutoNormalize???
//			do {
//				vk--;
//			}
//			while (!vk.Error() && !isKJVRef(vk));

			//hack to replace above:
			successive++;
			vk -= successive;
			orig = mod.getRawEntry();

			std::cerr << "Not a valid KJV ref: " << origVK << "\n";
			std::cerr << "appending to ref: " << vk << "\n";
			orig += " [ (";
			orig += origVK;
			orig += ") ";
			orig += verseText;
			orig += " ] ";
			verseText = orig.c_str();
		}
		else {
		  successive = 0;
		}

		if (orig.length() > 1)
			   std::cerr << "Warning, overwriting verse: " << vk << std::endl;
		  
		// ------------- End verse tests -----------------
		std::cout << "adding "<< vk << "\n";
		mod << verseText.c_str();	// save text to module at current position
	}

*/
