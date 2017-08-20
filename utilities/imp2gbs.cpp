/******************************************************************************
 *
 *  imp2gbs.cpp -    Utility to import GenBooks in IMP format
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifdef _MSC_VER
    #pragma warning( disable: 4251 )
#endif

#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <swordxx/config.h>
#include <swordxx/filemgr.h>
#include <swordxx/filters/utf8greekaccents.h>
#include <swordxx/keys/treekeyidx.h>
#include <swordxx/modules/common/entriesblk.h>
#include <swordxx/modules/genbook/rawgenbook.h>
#include <swordxx/stringmgr.h>
#include <swordxx/utilstr.h>
#if SWORDXX_HAS_ICU
#include <unicode/utypes.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/translit.h>
#include <unicode/locid.h>
#endif


using namespace swordxx;

std::string outPath;
std::string inFile;
bool  toUpper     = false;
bool  greekFilter = false;
bool  augMod      = false;
bool  augEnt      = true;
int   lexLevels   = 0;
UTF8GreekAccents greekAccentsFilter;


void usage(const char *app) {
    fprintf(stderr, "imp2gbs 1.0 General Book module creation tool for the Sword++Project\n\n");
    fprintf(stderr, "usage: %s <inFile> [OPTIONS]\n", app);
    fprintf(stderr, "\t-o <outPath>\n\t\tSpecify an output Path other than inFile location.\n");
    fprintf(stderr, "\t-a\n\t\tAugment Module [default: create new]\n");
    fprintf(stderr, "\t-O\n\t\tOverwrite entries of same key [default: append to]\n");
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
        else if (!strcmp(argv[i], "-O")) {
            augEnt = false;
        }
        else if (!strcmp(argv[i], "-a")) {
            augMod = true;
        }
        else if (!strcmp(argv[i], "-l")) {
            if (i+1 < argc) {
                lexLevels = atoi(argv[i+1]);
                i++;
            }
            if (!lexLevels) usage(*argv);
        }
    }
    if (outPath.empty()) {
        outPath = inFile;
        auto const lastPointIndex = outPath.rfind('.');
        if (lastPointIndex != std::string::npos)
            outPath.resize(lastPointIndex);
    }
}


void writeEntry(SWModule & book, std::string keyBuffer, std::string entBuffer) {


    if (greekFilter) {
        greekAccentsFilter.processText(keyBuffer);
    }

    if (toUpper)
        StringMgr::getSystemStringMgr()->upperUTF8(keyBuffer);

// Added for Hesychius, but this stuff should be pushed back into new StringMgr
// functionality
#if SWORDXX_HAS_ICU
//    if (lexLevels) {
    if (lexLevels && !hasPrefix(keyBuffer, "/Intro")) {
        unsigned size = (keyBuffer.size()+(lexLevels*2));
        keyBuffer.resize(size, '\0');

        UErrorCode err = U_ZERO_ERROR;

        int max = (size+5)*3;
        auto const ubuffer(std::make_unique<UChar[]>(max + 10u));
        int32_t len;

        u_strFromUTF8(ubuffer.get(), max+9, &len, keyBuffer.c_str(), -1, &err);
        if (err == U_ZERO_ERROR) {
            {
                auto const upper(
                            std::make_unique<UChar[]>((lexLevels + 1u) * 3u));
                memcpy(upper.get(), ubuffer.get(), lexLevels*sizeof(UChar));
                upper[lexLevels] = 0;
                len = u_strToUpper(upper.get(),
                                   (lexLevels + 1u) * 3u,
                                   upper.get(),
                                   -1,
                                   nullptr,
                                   &err);
                memmove(ubuffer.get() + len + 1u,
                        ubuffer.get(),
                        (max - len) * sizeof(UChar));
                memcpy(ubuffer.get(), upper.get(), len * sizeof(UChar));
                ubuffer[len] = '/';
            }

            int totalShift = 0;
            for (int i = lexLevels-1; i; i--) {
                int shift = (i < len)? i : len;
                memmove(ubuffer.get() + shift + 1,
                        ubuffer.get(),
                        (max - shift) * sizeof(UChar));
                ubuffer[shift] = '/';
                totalShift += (shift+1);
            }
            u_strToUTF8(&keyBuffer[0u], max, nullptr, ubuffer.get(), -1, &err);
        }
    }
#endif /* SWORDXX_HAS_ICU */

    std::cout << keyBuffer << std::endl;

    book.setKey(keyBuffer.c_str());

    // check to see if we already have an entry
    for (int i = 2; book.getKey()->popError() != KEYERR_OUTOFBOUNDS; ++i) {
        std::string key(formatted("%s {%d}", keyBuffer.c_str(), i));
        std::cout << "dup key, trying: " << key << std::endl;
        book.setKey(key.c_str());
    }

    book.setEntry(entBuffer.c_str());
}


int main(int argc, char **argv) {
    greekAccentsFilter.setOptionValue("Off");        // off = accents off
    parseParams(argc, argv);

    // Let's see if we can open our input file
    FileDesc *fd = FileMgr::getSystemFileMgr()->open(inFile.c_str(), FileMgr::RDONLY);
    if (fd->getFd() < 0) {
        fprintf(stderr, "error: %s: couldn't open input file: %s \n", argv[0], inFile.c_str());
        exit(-2);
    }

    // Do some initialization stuff
    if (!augMod) {
        RawGenBook::createModule(outPath.c_str());
    }
    {
        RawGenBook book(outPath.c_str());

        std::string lineBuffer;
        std::string keyBuffer;
        std::string entBuffer;

        while (!(lineBuffer = FileMgr::getLine(fd)).empty()) {
            if (hasPrefix(lineBuffer, "$$$")) {
                if ((keyBuffer.size()) && (entBuffer.size())) {
                    writeEntry(book, keyBuffer, entBuffer);
                }
                keyBuffer = lineBuffer;
                keyBuffer.erase(0u, 3u);
                trimString(keyBuffer);
                entBuffer.clear();
            }
            else {
                if (keyBuffer.size()) {
                    entBuffer += lineBuffer;
                    entBuffer += "\n";
                }
            }
        }
        if ((keyBuffer.size()) && (entBuffer.size())) {
            writeEntry(book, keyBuffer, entBuffer);
        }
    }

    FileMgr::getSystemFileMgr()->close(fd);

    return 0;
}



