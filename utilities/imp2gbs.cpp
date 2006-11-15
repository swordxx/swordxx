#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <entriesblk.h>
#include <iostream>
#include <treekeyidx.h>
#include <rawgenbook.h>
#include <utilstr.h>
#include <filemgr.h>
#include <utf8greekaccents.h>
#include <stringmgr.h>

//#ifdef _ICU_

#include <unicode/utypes.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <unicode/uchar.h>

#include <unicode/unistr.h>
#include <unicode/translit.h>

#include <unicode/locid.h>

//#endif

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

SWBuf outPath;
SWBuf inFile;
bool  toUpper     = false;
bool  greekFilter = false;
int   lexLevels   = 0;
UTF8GreekAccents greekAccentsFilter;


void usage(const char *app) {
	fprintf(stderr, "imp2gbs 1.0 General Book module creation tool for the SWORD Project\n\n");
	fprintf(stderr, "usage: %s <inFile> [OPTIONS]\n", app);
	fprintf(stderr, "\t-o <outPath>\n\t\tSpecify an output Path other than inFile location.\n");
	fprintf(stderr, "\t-U\n\t\tKey filter: Convert toUpper\n");
	fprintf(stderr, "\t-g\n\t\tKey filter: Strip Greek diacritics\n");
	fprintf(stderr, "\t-l <levels>\n\t\tKey filter: Pseudo-Lexicon n-level generation using first character\n");
	fprintf(stderr, "\t\te.g. -l 2 \"Abbey\" -> \"A/AB/Abbey\"\n");
	fprintf(stderr, "\n");
	exit (-1);
}


void parseParams(int argc, char **argv) {

	if (argc < 2) {
		usage(*argv);
	}

	inFile = argv[1];

	for (int i = 2; i < argc; i++) {
		if (!strcmp(argv[i], "-o")) {
			if ((i+1 < argc) && (argv[i+1][0] != '-')) {
				outPath = argv[i+1];
				i++;
			}
			else usage(*argv);
		}
		else if (!strcmp(argv[i], "-U")) {
			if (StringMgr::hasUTF8Support()) {
				toUpper = true;
			}
			else {
				fprintf(stderr, "Error: %s.  Cannot reliably toUpper without UTF8 support\n\t(recompile with ICU enabled)\n\n", *argv);
				usage(*argv);
			}
		}
		else if (!strcmp(argv[i], "-g")) {
			greekFilter = true;
		}
		else if (!strcmp(argv[i], "-l")) {
			if (i+1 < argc) {
				lexLevels = atoi(argv[i+1]);
				i++;
			}
			if (!lexLevels) usage(*argv);
		}
	}
	if (!outPath.size()) {
		outPath = inFile;
		int i;
		for (i = 0; (i < outPath.size() && outPath[i] != '.'); i++);
		outPath.size(i);
	}
}
  

void writeEntry(SWModule *book, SWBuf keyBuffer, SWBuf entBuffer) {


	if (greekFilter) {
		greekAccentsFilter.processText(keyBuffer);
	}

	if (toUpper) {
		unsigned size = (keyBuffer.size()+5)*3;
		keyBuffer.setFillByte(0);
		keyBuffer.resize(size);
		StringMgr::getSystemStringMgr()->upperUTF8(keyBuffer.getRawData(), size-2);
	}

//#ifdef _ICU_
	if (lexLevels) {
		unsigned size = (keyBuffer.size()+(lexLevels*2));
		keyBuffer.setFillByte(0);
		keyBuffer.resize(size);
			
		UErrorCode err = U_ZERO_ERROR;
		
		int max = (size+5)*3;
		UChar *ubuffer = new UChar[max+10];
		int32_t len;
		
		u_strFromUTF8(ubuffer, max+9, &len, keyBuffer.c_str(), -1, &err);
		if (err == U_ZERO_ERROR) {
			int totalShift = 0;
			for (int i = lexLevels; i; i--) {
				int shift = (i < len)? i : len;
				memmove(ubuffer+(shift+1), ubuffer, (max-shift)*sizeof(UChar));
				ubuffer[shift] = '/';
				totalShift += (shift+1);
			}
			UChar *upper = new UChar[(totalShift+1)*3];
			memcpy(upper, ubuffer, totalShift*sizeof(UChar));
			upper[totalShift] = 0;
			len = u_strToUpper(upper, (totalShift+1)*3, upper, -1, 0, &err);
			memmove(ubuffer+len, ubuffer+totalShift, (max-totalShift)*sizeof(UChar));
			memcpy(ubuffer, upper, len*sizeof(UChar));
			delete [] upper;
			u_strToUTF8(keyBuffer.getRawData(), max, 0, ubuffer, -1, &err);
		}
		
		delete [] ubuffer;
	}
//#endif

	std::cout << keyBuffer << std::endl;
	book->setKey(keyBuffer.c_str());
	book->setEntry(entBuffer, strlen(entBuffer));
}


int main(int argc, char **argv) {
	greekAccentsFilter.setOptionValue("Off");		// off = accents off
	parseParams(argc, argv);
  
	// Let's see if we can open our input file
	FileDesc *fd = FileMgr::getSystemFileMgr()->open(inFile, FileMgr::RDONLY);
	if (fd->getFd() < 0) {
		fprintf(stderr, "error: %s: couldn't open input file: %s \n", argv[0], inFile.c_str());
		exit(-2);
	}
  
	RawGenBook *book;
  
	// Do some initialization stuff
	RawGenBook::createModule(outPath);
	book = new RawGenBook(outPath);
  
	SWBuf lineBuffer;
	SWBuf keyBuffer;
	SWBuf entBuffer;

	bool more = true;
	do {
		more = FileMgr::getLine(fd, lineBuffer);
		if (lineBuffer.startsWith("$$$")) {
			if ((keyBuffer.size()) && (entBuffer.size())) {
				writeEntry(book, keyBuffer, entBuffer);
			}
			keyBuffer = lineBuffer;
			keyBuffer << 3;
			keyBuffer.trim();
			entBuffer.size(0);
		}
		else {
			if (keyBuffer.size()) {
				entBuffer += lineBuffer;
				entBuffer += "\n";
			}
		}
	} while (more);

	delete book;

	FileMgr::getSystemFileMgr()->close(fd);

	return 0;
}



