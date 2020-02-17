/******************************************************************************
 *
 *  imp2gbs.cpp -    Utility to import GenBooks in IMP format
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

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <swordxx/filemgr.h>
#include <swordxx/filters/utf8greekaccents.h>
#include <swordxx/keys/treekeyidx.h>
#include <swordxx/max_v.h>
#include <swordxx/modules/genbook/rawgenbook.h>
#include <swordxx/utilstr.h>
#include <unicode/utypes.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/translit.h>
#include <unicode/locid.h>


using namespace swordxx;
using namespace std::literals::string_view_literals;

namespace {

std::string outPath;
std::string inFile;
bool  toUpper     = false;
bool  greekFilter = false;
bool  augMod      = false;
bool  augEnt      = true;
std::size_t lexLevels   = 0;
UTF8GreekAccents greekAccentsFilter;


[[noreturn]] void usage(char const * const app) {
    std::cerr <<
            "imp2gbs 1.0 General Book module creation tool for the Sword++ "
                "project\n\n"
            "usage: " << app << " <inFile> [OPTIONS]\n"
            "\t-o <outPath>\n\t\tSpecify an output Path other than inFile "
                "location.\n"
            "\t-a\n\t\tAugment Module [default: create new]\n"
            "\t-O\n\t\tOverwrite entries of same key [default: append to]\n"
            "\t-U\n\t\tKey filter: Convert toUpper\n"
            "\t-g\n\t\tKey filter: Strip Greek diacritics\n"
            "\t-l <levels>\n"
                "\t\tKey filter: Pseudo-Lexicon n-level generation using first "
                "character\n"
            "\t\te.g. -l 2 \"Abbey\" -> \"A/AB/Abbey\"\n\n" << std::flush;
    std::exit(EXIT_FAILURE);
}


void parseParams(int argc, char * argv[]) {
    if (argc < 2)
        usage(*argv);

    inFile = argv[1];

    for (int i = 2; i < argc; i++) {
        if (!std::strcmp(argv[i], "-o")) {
            if ((i + 1 < argc) && (argv[i + 1][0] != '-')) {
                outPath = argv[i+1];
                i++;
            }
            else usage(*argv);
        } else if (!std::strcmp(argv[i], "-U")) {
            toUpper = true;
        } else if (!std::strcmp(argv[i], "-g")) {
            greekFilter = true;
        } else if (!std::strcmp(argv[i], "-O")) {
            augEnt = false;
        } else if (!std::strcmp(argv[i], "-a")) {
            augMod = true;
        } else if (!std::strcmp(argv[i], "-l")) {
            int arg = 0;
            if (i + 1 < argc) {
                arg = std::atoi(argv[i + 1]);
                static_assert(max_v<int> <= max_v<std::size_t>, "");
                if (arg <= 0)
                    usage(*argv);
                lexLevels = static_cast<std::size_t>(arg);
                i++;
            } else {
                usage(*argv);
            }
        }
    }
    if (outPath.empty()) {
        outPath = inFile;
        auto const lastPointIndex = outPath.rfind('.');
        if (lastPointIndex != std::string::npos)
            outPath.resize(lastPointIndex);
    }
}

void writeEntry(SWModule & book,
                std::string keyBuffer,
                std::string const & entBuffer)
{
    if (greekFilter)
        greekAccentsFilter.processText(keyBuffer);
    if (toUpper)
        keyBuffer = utf8ToUpper(keyBuffer);

// Added for Hesychius, but this stuff should be pushed back into new StringMgr
// functionality
//    if (lexLevels) {
    if (lexLevels && !startsWith(keyBuffer, "/Intro"sv)) {
        auto const size = (keyBuffer.size() + (lexLevels * 2));
        keyBuffer.resize(size, '\0');

        UErrorCode err = U_ZERO_ERROR;

        auto const max = (size + 5u) * 3u;
        auto const ubuffer(std::make_unique<UChar[]>(max + 10u));
        int32_t len;

        u_strFromUTF8(ubuffer.get(),
                      max + 9u,
                      &len,
                      keyBuffer.c_str(),
                      -1,
                      &err);
        assert(len >= 0);
        static_assert(max_v<std::size_t> >= max_v<std::int32_t>, "");
        auto const ulen = static_cast<std::size_t>(len);
        if (err == U_ZERO_ERROR) {
            {
                auto const upper(
                            std::make_unique<UChar[]>((lexLevels + 1u) * 3u));
                std::memcpy(upper.get(), ubuffer.get(), lexLevels * sizeof(UChar));
                upper[lexLevels] = 0;
                len = u_strToUpper(upper.get(),
                                   (lexLevels + 1u) * 3u,
                                   upper.get(),
                                   -1,
                                   nullptr,
                                   &err);
                std::memmove(ubuffer.get() + len + 1u,
                             ubuffer.get(),
                             (max - ulen) * sizeof(UChar));
                std::memcpy(ubuffer.get(), upper.get(), ulen * sizeof(UChar));
                ubuffer[ulen] = '/';
            }

            int totalShift = 0;
            for (std::size_t i = lexLevels - 1u; i; --i) {
                auto const shift = (i < ulen)? i : ulen;
                std::memmove(ubuffer.get() + shift + 1,
                             ubuffer.get(),
                             (max - shift) * sizeof(UChar));
                ubuffer[shift] = '/';
                totalShift += (shift+1);
            }
            u_strToUTF8(&keyBuffer[0u], max, nullptr, ubuffer.get(), -1, &err);
        }
    }

    std::cout << keyBuffer << std::endl;

    book.getKey()->positionFrom(keyBuffer.c_str());

    // check to see if we already have an entry
    for (int i = 2; book.getKey()->popError() != KEYERR_OUTOFBOUNDS; ++i) {
        std::string key(formatted("%s {%d}", keyBuffer, i));
        std::cout << "dup key, trying: " << key << std::endl;
        book.getKey()->positionFrom(key.c_str());
    }

    book.setEntry(entBuffer);
}

} // anonymous namespace

int main(int argc, char * argv[]) {
    greekAccentsFilter.setOptionValue("Off"); // off = accents off
    parseParams(argc, argv);

    // Let's see if we can open our input file
    auto const fd(FileMgr::getSystemFileMgr()->open(inFile.c_str(), FileMgr::RDONLY));
    if (fd->getFd() < 0) {
        std::cerr << "Error: " << argv[0] << ": couldn't open input file: "
                  << inFile << " \n";
        std::exit(EXIT_FAILURE);
    }

    // Do some initialization stuff
    if (!augMod)
        RawGenBook::createModule(outPath);
    {
        RawGenBook book(outPath);

        std::string lineBuffer;
        std::string keyBuffer;
        std::string entBuffer;

        while (!(lineBuffer = fd->getLine()).empty()) {
            if (startsWith(lineBuffer, "$$$"sv)) {
                if (!keyBuffer.empty() && !entBuffer.empty())
                    writeEntry(book, keyBuffer, entBuffer);
                keyBuffer = lineBuffer;
                keyBuffer.erase(0u, 3u);
                trimString(keyBuffer);
                entBuffer.clear();
            } else {
                if (!keyBuffer.empty()) {
                    entBuffer += lineBuffer;
                    entBuffer += "\n";
                }
            }
        }
        if (!keyBuffer.empty() && !entBuffer.empty())
            writeEntry(book, keyBuffer, entBuffer);
    }
}
