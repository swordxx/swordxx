/******************************************************************************
 *
 *  mod2zmod.cpp -    Compression on variable granularity
 *
 * Copyright 2000-2014 CrossWire Bible Society (http://www.crosswire.org)
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
#include <cstring>
#include <fcntl.h>
#include <fstream>
#ifndef __GNUC__
#include <io.h>
#endif
#include <iostream>
#include <memory>
#include <string>
#include <swordxx/filters/cipherfil.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/modules/comments/zcom.h>
#include <swordxx/modules/common/bz2comprs.h>
#include <swordxx/modules/common/lzsscomprs.h>
#include <swordxx/modules/common/xzcomprs.h>
#include <swordxx/modules/common/zipcomprs.h>
#include <swordxx/modules/lexdict/zld.h>
#include <swordxx/modules/texts/ztext.h>
#include <swordxx/swmgr.h>
#include <swordxx/swtext.h>
#ifdef __GNUC__
#include <unistd.h>
#endif
#include <utility>


using namespace swordxx;

using std::cerr;
using std::string;
using std::cout;

void errorOutHelp(char *appName) {
    cerr << appName << " - a tool to create compressed Sword++ modules\n";
    cerr << "version 0.1\n\n";
    cerr << "usage: "<< appName << " <modname> <datapath> [blockType [compressType [compressLevel [cipherKey]]]]\n\n";
    cerr << "datapath: the directory in which to write the zModule\n";
    cerr << "blockType  : (default 4)\n\t2 - verses\n\t3 - chapters\n\t4 - books\n";
    cerr << "compressType: (default 1):\n\t1 - LZSS\n\t2 - Zip\n\t3 - bzip2\n\t4 - xz\n";
    cerr << "compressLevel: (default varies by compressType):\n\tA digit from 1-9. Greater values compress more, but require more\n\ttime/memory. Use 0 for the default compression level.\n";
    cerr << "\n\n";
    std::exit(-1);
}


int main(int argc, char **argv)
{
    BlockType iType = BOOKBLOCKS;
    int compType = 1;
    string cipherKey = "";
    std::unique_ptr<SWCompress> compressor;

    int compLevel = 0;

    if ((argc < 3) || (argc > 7)) {
        errorOutHelp(argv[0]);
    }

    if (argc > 3) {
        switch (std::atoi(argv[3])) {
        case 2: iType = VERSEBLOCKS; break;
        case 3: iType = CHAPTERBLOCKS; break;
        case 4: iType = BOOKBLOCKS; break;
        default:
            std::cerr << "Argument 3 must be one of <2|3|4>\n";
            errorOutHelp(*argv);
        }
        if (argc > 4) {
            compType = std::atoi(argv[4]);
            if (argc > 5) {
                compLevel = std::atoi(argv[5]);
                if (argc > 6) {
                    cipherKey = argv[6];
                }
            }
        }
    }

    if ((iType < 2) || (compType < 1) || (compType > 4) || compLevel < 0 || compLevel > 9 || (!std::strcmp(argv[1], "-h")) || (!std::strcmp(argv[1], "--help")) || (!std::strcmp(argv[1], "/?")) || (!std::strcmp(argv[1], "-?")) || (!std::strcmp(argv[1], "-help"))) {
        errorOutHelp(argv[0]);
    }

    SWMgr mgr;

    auto const it = mgr.modules().find(argv[1]);
    if (it == mgr.modules().end()) {
        fprintf(stderr, "error: %s: couldn't find module: %s\n", argv[0], argv[1]);
        std::exit(-2);
    }

    SWModule & inModule = *it->second;

    // Try to initialize a default set of datafiles and indicies at our
    // datapath location passed to us from the user.

#define BIBLE 1
#define LEX 2
#define COM 3

    int modType = 0;
    if (inModule.getType() == "Biblical Texts") {
        modType = BIBLE;
    } else if (inModule.getType() == "Lexicons / Dictionaries") {
        modType = LEX;
    } else if (inModule.getType() == "Commentaries") {
        modType = COM;
    }

    switch (compType) {    // these are deleted by zText
    case 1: compressor = std::make_unique<LZSSCompress>(); break;
    case 2: compressor = std::make_unique<ZipCompress>(); break;
    case 3: compressor = std::make_unique<Bzip2Compress>(); break;
    case 4: compressor = std::make_unique<XzCompress>(); break;
    }
    if (compressor && compLevel > 0) {
        compressor->setLevel(compLevel);
    }

    int result = 0;
    switch (modType) {
    case BIBLE:
    case COM: {
        auto const vk(inModule.getKeyAs<VerseKey>());
        result = zText::createModule(argv[2], iType, vk->versificationSystem());
        break;
    }
    case LEX:
        result = zLD::createModule(argv[2]);
        break;
    }

    if (result) {
        fprintf(stderr, "error: %s: couldn't create module at path: %s\n", argv[0], argv[2]);
        std::exit(-3);
    }

    std::unique_ptr<SWModule> outModule;
    switch (modType) {
    case BIBLE:
    case COM: {
        auto const vk(inModule.getKeyAs<VerseKey>());
        outModule = std::make_unique<zText>(argv[2], nullptr, nullptr, iType, std::move(compressor),
            ENC_UNKNOWN, DIRECTION_LTR, FMT_UNKNOWN, nullptr,
            vk->versificationSystem());    // open our datapath with our RawText driver.
        vk->setIntros(true);
        break;
    }
    case LEX:
        outModule = std::make_unique<zLD>(argv[2], nullptr, nullptr, iType, std::move(compressor));        // open our datapath with our RawText driver.
        break;
    }

    if (!cipherKey.empty())
        outModule->addRawFilter(
                    std::make_shared<CipherFilter>(cipherKey.c_str()));

    string lastBuffer = "Something that would never be first module entry";
    SWKey bufferKey;
    SWKey lastBufferKey;
    auto const outModuleKey(outModule->createKey());
    VerseKey *vkey = dynamic_cast<VerseKey *>(outModuleKey.get());
    if (vkey) {
        vkey->setIntros(true);
        vkey->setAutoNormalize(false);
    }
    outModule->setKey(outModuleKey);

    inModule.setSkipConsecutiveLinks(false);
    inModule.positionToTop();
    while (!inModule.popError()) {
        bufferKey.positionFrom(*inModule.getKey());
        // pseudo-check for link.  Will get most common links.
        if ((lastBuffer == inModule.getRawEntry()) &&(lastBuffer.length() > 0)) {
            outModuleKey->positionFrom(bufferKey);
            outModule->linkEntry(lastBufferKey);    // link to last key
        cout << "Adding [" << bufferKey.getText() << "] link to: [" << lastBufferKey.getText() << "]\n";
        }
        else {
            lastBuffer = inModule.getRawEntry();
            lastBufferKey.setText(inModule.getKeyText());
            if (lastBuffer.length() > 0) {
                cout << "Adding [" << bufferKey.getText() << "] new text.\n";
                outModuleKey->positionFrom(bufferKey);
//                outModule->getRawEntry();    // snap
//                outModule->setKey(bufferKey);
                outModule->setEntry(lastBuffer);    // save new text;
            }
            else {
                    cout << "Skipping [" << bufferKey.getText() << "] no entry in Module.\n";
            }
        }
        inModule.increment();
    }
}

