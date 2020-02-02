/******************************************************************************
 *
 *  vpl2mod.cpp -    Utility to import VPL formatted modules
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#ifndef __GNUC__
#include <io.h>
#endif
#include <memory>
#include <string>
#include <swordxx/filemgr.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/modules/texts/rawtext.h>
#include <swordxx/swmgr.h>
#include <swordxx/utilstr.h>
#include <sys/stat.h>
#ifdef __GNUC__
#include <unistd.h>
#endif
#include <utility>


using swordxx::FileMgr;
using swordxx::SWMgr;
using swordxx::RawText;
using swordxx::VerseKey;


std::pair<bool, std::unique_ptr<char[]>> readline(int fd) {
    char ch;
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

    if (size > 0) {
        auto buf(std::make_unique<char[]>(size + 1u));
        lseek(fd, index, SEEK_SET);
        read(fd, buf.get(), size);
        read(fd, &ch, 1);   //pop terminating char
        buf[size] = 0;

        // clean up any trailing junk on buf
        for (char *it = buf.get() + (std::strlen(buf.get()) - 1u); it > buf.get(); it--) {
            if ((*it != 10) && (*it != 13) && (*it != ' ') && (*it != '\t'))
                break;
            else *it = 0;
        }
        return {!len, std::move(buf)};
    }
    return {!len, nullptr};
}


char *parseVReg(char *buf) {
    char stage = 0;

    while (*buf) {
        switch (stage) {
        case 0:
            if (swordxx::charIsAlpha(*buf))
                stage++;
            break;
        case 1:
            if (swordxx::charIsDigit(*buf))
                stage++;
            break;
        case 2:
            if (*buf == ':')
                stage++;
            break;
        case 3:
            if (swordxx::charIsDigit(*buf))
                stage++;
            break;
       case 4:
            if (*buf == ' ') {
                *buf = 0;
                return ++buf;
            }
            break;
        }
        buf++;
    }
    // if we got to stage 4 return after key buf, else return 0;
    return (stage == 4) ? buf : nullptr;
}


bool isKJVRef(const char *buf) {
    VerseKey vk, test;
    vk.setAutoNormalize(false);
    vk.setIntros(true);    // turn on mod/testmnt/book/chap headings
    // lets do some tests on the verse --------------
    vk.setText(buf);
    test.setText(buf);

    if (vk.getTestament() && vk.getBook() && vk.getChapter() && vk.getVerse()) { // if we're not a heading
//        std::cerr << (const char*)vk << " == "  << (const char*)test << std::endl;
        return (vk == test);
    }
    else return true;    // no check if we're a heading... Probably bad.
}


void fixText(char *text) {
    char *to = text;
    while(*text) {
        *to++ = *text++;
        *to++ = *text++;
        if (!*text)
            break;
        if (*text != ' ')
            std::cerr << "problem\n";
        else    text++;
    }
    *to = 0;
}

int main(int argc, char **argv) {

    // Let's test our command line arguments
    if (argc < 2) {
//        fprintf(stderr, "usage: %s <vpl_file> </path/to/mod> [0|1 - file includes prepended verse references]\n", argv[0]);
        fprintf(stderr, "usage: %s <source_vpl_file> </path/to/output/mod/> [0|1 - prepended verse refs] [0|1 - NT only]\n\n", argv[0]);
        fprintf(stderr, "\tWARNING: THIS IS CURRENTLY A KJV-VERSIFICATION-ONLY UTILITY\n");
        fprintf(stderr, "\tWith no verse refs, source file must contain exactly 31102 lines.\n");
        fprintf(stderr, "\tThis is KJV verse count plus headings for MODULE,\n");
        fprintf(stderr, "\tTESTAMENT, BOOK, CHAPTER. An example snippet follows:\n\n");
        fprintf(stderr, "\t\tMODULE HEADER\n");
        fprintf(stderr, "\t\tOLD TESTAMENT HEADER\n");
        fprintf(stderr, "\t\tGENESIS HEADER\n");
        fprintf(stderr, "\t\tCHAPTER 1 HEADER\n");
        fprintf(stderr, "\t\tIn the beginning...\n\n");
        fprintf(stderr, "\t... implying there must also be a CHAPTER2 HEADER,\n");
        fprintf(stderr, "\tEXODUS HEADER, NEW TESTAMENT HEADER, etc.  If there is no text for\n");
        fprintf(stderr, "\tthe header, a blank line must, at least, hold place.\n\n");
        fprintf(stderr, "\tWith verse refs, source file must simply contain any number of lines,\n");
        fprintf(stderr, "\tthat begin with the verse reference for which it is an entry.  e.g.:\n\n");
        fprintf(stderr, "\t\tgen 1:0 CHAPTER 1 HEADER\n");
        fprintf(stderr, "\t\tgen 1:1 In the beginning...\n\n");
        std::exit(-1);
    }

    // Let's see if we can open our input file
    int fd = FileMgr::openFileReadOnly(argv[1]);
    if (fd < 0) {
        fprintf(stderr, "error: %s: couldn't open input file: %s \n", argv[0], argv[1]);
        std::exit(-2);
    }

    // Try to initialize a default set of datafiles and indicies at our
    // datapath location passed to us from the user.
    if (RawText::createModule(argv[2])) {
        fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
        std::exit(-3);
    }

    // not used yet, but for future support of a vpl file with each line
    // prepended with verse reference, eg. "Gen 1:1 In the beginning..."
    bool vref = false;
    if (argc > 3)
        vref = (argv[3][0] == '0') ? false : true;

    // if 'nt' is the 4th arg, our vpl file only has the NT
    bool ntonly = false;
    if (argc > 4)
                ntonly = (argv[4][0] == '0') ? false : true;

    // Do some initialization stuff
    RawText mod(argv[2]);    // open our datapath with our RawText driver.
    auto vk(std::make_shared<VerseKey>());
    vk->setAutoNormalize(false);
    vk->setIntros(true);    // turn on mod/testmnt/book/chap headings

    mod.setKey(vk);

    // Loop through module from TOP to BOTTOM and set next line from
    // input file as text for this entry in the module
    mod.positionToTop();
    if (ntonly) vk->setText("Matthew 1:1");

    int successive = 0;  //part of hack below
    while ((!mod.popError())) {
        auto r(readline(fd));
        if (!r.first)
            break;
        auto const & buffer = r.second;
        if (buffer[0u] == '|')    // comments, ignore line
            continue;
        if (vref) {
            const char *verseText = parseVReg(buffer.get());
            if (!verseText) {    // if we didn't find a valid verse ref
                std::cerr << "No valid verse ref found on line: " << buffer.get() << "\n";
                std::exit(-4);
            }

            vk->setText(buffer.get());
            if (vk->popError()) {
                std::cerr << "Error parsing key: " << buffer.get() << "\n";
                std::exit(-5);
            }
            std::string orig = mod.getRawEntry();

            if (!isKJVRef(buffer.get())) {
                VerseKey origVK(*vk);
                /* This block is functioning improperly -- problem with AutoNormalize???
                do {
                    vk--;
                }
                while (!vk.popError() && !isKJVRef(vk)); */
                //hack to replace above:
                successive++;
                vk->decrement(successive);
                orig = mod.getRawEntry();

                std::cerr << "Not a valid KJV ref: " << origVK.getText() << "\n";
                std::cerr << "appending to ref: " << vk->getText() << "\n";
                orig += " [ (";
                orig += origVK.getText();
                orig += ") ";
                orig += verseText;
                orig += " ] ";
                verseText = orig.c_str();
            }
            else {
              successive = 0;
            }

            if (orig.length() > 1)
                   std::cerr << "Warning, overwriting verse: " << vk->getText() << std::endl;

            // ------------- End verse tests -----------------
            mod.setEntry(verseText);    // save text to module at current position
        }
        else {
            fixText(buffer.get());
            mod.setEntry(buffer.get());    // save text to module at current position
            mod.increment();    // increment module position
        }
    }
}
