/******************************************************************************
 *
 *  imp2vs.cpp -    Utility to import IMP formatted VerseKey modules
 *
 * Copyright 2002-2014 CrossWire Bible Society (http://www.crosswire.org)
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
#include <iostream>
#include <string>
#include <swordxx/filemgr.h>
#include <swordxx/filters/cipherfil.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/localemgr.h>
#include <swordxx/modules/common/bz2comprs.h>
#include <swordxx/modules/common/lzsscomprs.h>
#include <swordxx/modules/common/xzcomprs.h>
#include <swordxx/modules/common/zipcomprs.h>
#include <swordxx/modules/texts/rawtext.h>
#include <swordxx/modules/texts/rawtext4.h>
#include <swordxx/modules/texts/ztext.h>
#include <swordxx/utilstr.h>


using namespace swordxx;

using namespace std;

void writeEntry(SWModule & module,
                std::string const & key,
                std::string const & entry,
                bool const replace);

[[noreturn]] void usage(char const * progName, char const * error = nullptr) {
    if (error) fprintf(stderr, "\n%s: %s\n", progName, error);
    fprintf(stderr, "\n=== imp2vs (Revision $Rev$) Sword++ Bible/Commentary importer.\n");
    fprintf(stderr, "\nusage: %s <imp_file> [options]\n", progName);
    fprintf(stderr, "  -a\t\t\t augment module if exists (default is to create new)\n");
    fprintf(stderr, "  -r\t\t\t replace existing entries (default is to append)\n");
    fprintf(stderr, "  -z <l|z|b|x>\t\t use compression (default: none):\n");
    fprintf(stderr, "\t\t\t\t l - LZSS; z - ZIP; b - bzip2; x - xz\n");
    fprintf(stderr, "  -o <output_path>\t where to write data files.\n");
    fprintf(stderr, "  -4\t\t\t use 4 byte size entries (default is 2).\n");
    fprintf(stderr, "  -b <2|3|4>\t\t compression block size (default 4):\n");
    fprintf(stderr, "\t\t\t\t 2 - verse; 3 - chapter; 4 - book\n");
    fprintf(stderr, "  -v <v11n>\t\t specify a versification scheme to use (default is KJV)\n");
    fprintf(stderr, "\t\t\t\t Note: The following are valid values for v11n:\n");
    {
        auto const vmgr(VersificationMgr::systemVersificationMgr());
        for (auto const & vs : vmgr->getVersificationSystems())
            fprintf(stderr, "\t\t\t\t\t%s\n", vs.c_str());
    }
    fprintf(stderr, "  -l <locale>\t\t specify a locale scheme to use (default is en)\n");
    fprintf(stderr, "  -c <cipher_key>\t encipher module using supplied key\n");
    fprintf(stderr, "\t\t\t\t (default no enciphering)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "'imp' format is a simple standard for importing data into Sword++ modules.\n"
        "Required is a plain text file containing $$$key lines followed by content.\n\n"
        "$$$Gen.1.1\n"
        "In the beginning God created\n"
        "the heavens and the earth\n"
        "$$$Gen.1.2\n"
        "and the earth...\n\n"
        "Key lines can contain ranges, for example, a commentary entry which discusses\n"
        "John 1:1-4 might have a key, $$$Jn.1.1-4.  Special keys for intro entries use\n"
        "standard Sword++ notation, e.g. $$$Rom.4.0 for intro of Romans chapter 4,\n"
        "$$$Rev.0.0 for intro of the Book of Revelation of John.  $$$[ Module Heading ]\n"
        "for entire module intro.  $$$[ Testament 2 Heading ] for NT intro.\n\n");
    std::exit(-1);
}


int main(int argc, char **argv) {
    // handle options
    if (argc < 2) usage(*argv);

    const char *progName   = argv[0];
    const char *inFileName = argv[1];
    std::shared_ptr<VersificationMgr::System const> v11n(
            VersificationMgr::systemVersificationMgr()->getVersificationSystem(
                "KJV"));
    std::string outPath      = "./";
    std::string locale           = "en";

    bool fourByteSize      = false;
    bool append        = false;
    bool replace = false;
    BlockType iType = BOOKBLOCKS;
    std::string cipherKey;
    std::string compType;

    for (int i = 2; i < argc; i++) {
        if (!std::strcmp(argv[i], "-a")) {
            append = true;
        } else if (!std::strcmp(argv[i], "-r")) {
            replace = true;
        } else if (!std::strcmp(argv[i], "-z")) {
            if (fourByteSize) usage(*argv, "Cannot specify both -z and -4");
            compType = "ZIP";
            if (i+1 < argc && argv[i+1][0] != '-') {
                switch (argv[++i][0]) {
                case 'l': compType = "LZSS"; break;
                case 'z': compType = "ZIP"; break;
                case 'b': compType = "BZIP2"; break;
                case 'x': compType = "XZ"; break;
                }
            }
        }
        else if (!std::strcmp(argv[i], "-Z")) {
            if (compType.size()) usage(*argv, "Cannot specify both -z and -Z");
            if (fourByteSize) usage(*argv, "Cannot specify both -Z and -4");
            compType = "LZSS";
        }
        else if (!std::strcmp(argv[i], "-4")) {
            fourByteSize = true;
        }
        else if (!std::strcmp(argv[i], "-b")) {
            if (i+1 < argc) {
                switch (std::atoi(argv[++i])) {
                case 2: iType = VERSEBLOCKS; continue;
                case 3: iType = CHAPTERBLOCKS; continue;
                case 4: iType = BOOKBLOCKS; continue;
                default: break;
                }
            }
            usage(*argv, "-b requires one of <2|3|4>");
        }
        else if (!std::strcmp(argv[i], "-o")) {
            if (i+1 < argc) outPath = argv[++i];
            else usage(progName, "-o requires <output_path>");
        }
        else if (!std::strcmp(argv[i], "-v")) {
            if (i + 1 < argc) {
                auto v(VersificationMgr::systemVersificationMgr()->getVersificationSystem(argv[++i]));
                if (v) {
                    v11n = std::move(v);
                } else {
                    fprintf(stderr, "ERROR: %s: Requested versification system not found: %s \n", *argv, argv[i]);
                    std::exit(-1);
                }
            } else {
                usage(progName, "-v requires <v11n>");
            }
        }
        else if (!std::strcmp(argv[i], "-l")) {
            if (i+1 < argc) locale = argv[++i];
            else usage(progName, "-l requires <locale>");
        }
        else if (!std::strcmp(argv[i], "-c")) {
            if (i+1 < argc) cipherKey = argv[++i];
            else usage(*argv, "-c requires <cipher_key>");
        }
        else usage(progName, (((std::string)"Unknown argument: ")+ argv[i]).c_str());
    }
    // -----------------------------------------------------

    std::unique_ptr<SWCompress> compressor;
    if (compType == "LZSS") {
        compressor = std::make_unique<LZSSCompress>();
    }
    else if (compType == "ZIP") {
        compressor = std::make_unique<ZipCompress>();
    }
    else if (compType == "BZIP2") {
        compressor = std::make_unique<Bzip2Compress>();
    }
    else if (compType == "XZ") {
        compressor = std::make_unique<XzCompress>();
    }


    // setup module
    if (!append) {
        if (compressor) {
            if (zText::createModule(outPath.c_str(), iType, v11n)) {
                fprintf(stderr, "ERROR: %s: couldn't create module at path: %s \n", *argv, outPath.c_str());
                std::exit(-1);
            }
        }
        else {
            if (!fourByteSize)
                RawText::createModule(outPath.c_str(), v11n);
            else    RawText4::createModule(outPath.c_str(), v11n);
        }
    }

    std::unique_ptr<SWModule> module;
    if (compressor) {
        // Create a compressed text module allowing very large entries
        // Taking defaults except for first, fourth, fifth and last argument
        module = std::make_unique<zText>(
                outPath.c_str(),        // ipath
                nullptr,        // iname
                nullptr,        // idesc
                iType,        // iblockType
                std::move(compressor),    // icomp
                ENC_UNKNOWN,    // enc
                DIRECTION_LTR,    // dir
                FMT_UNKNOWN,    // markup
                nullptr,        // lang
                v11n         // versification system
               );
    } else if (fourByteSize) {
        module = std::make_unique<RawText4>(outPath.c_str(),
                                            nullptr,
                                            nullptr,
                                            ENC_UNKNOWN,
                                            DIRECTION_LTR,
                                            FMT_UNKNOWN,
                                            nullptr,
                                            v11n);
    } else {
        module = std::make_unique<RawText>(outPath.c_str(),
                                           nullptr,
                                           nullptr,
                                           ENC_UNKNOWN,
                                           DIRECTION_LTR,
                                           FMT_UNKNOWN,
                                           nullptr,
                                           v11n);
    }
    // -----------------------------------------------------

    if (!cipherKey.empty()) {
       fprintf(stderr, "Adding cipher filter with phrase: %s\n", cipherKey.c_str());
       module->addRawFilter(std::make_shared<CipherFilter>(cipherKey.c_str()));
    }

    // setup locale manager

    LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(locale.c_str());


    // setup module key to allow full range of possible values, and then some

    std::shared_ptr<VerseKey> vkey(
                std::static_pointer_cast<VerseKey>(module->createKey()));
    vkey->setIntros(true);
    vkey->setAutoNormalize(false);
    module->setKey(vkey);
    // -----------------------------------------------------


    // process input file
    auto const fd(FileMgr::getSystemFileMgr()->open(inFileName, FileMgr::RDONLY));

    std::string lineBuffer;
    std::string keyBuffer;
    std::string entBuffer;

    using namespace std::literals::string_view_literals;
    while (!(lineBuffer = fd->getLine()).empty()) {
        if (startsWith(lineBuffer, "$$$"sv)) {
            if ((keyBuffer.size()) && (entBuffer.size())) {
                writeEntry(*module, keyBuffer, entBuffer, replace);
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
    };
    if ((keyBuffer.size()) && (entBuffer.size())) {
        writeEntry(*module, keyBuffer, entBuffer, replace);
    }
}

void writeEntry(SWModule & module,
                std::string const & key,
                std::string const & entry,
                bool const replace)
{
    if (key.size() && entry.size()) {
        std::cout << "from file: " << key << std::endl;
        auto const vkey(module.getKeyAs<VerseKey>());
        std::shared_ptr<VerseKey> linkMaster(
                    std::static_pointer_cast<VerseKey>(module.createKey()));

        ListKey listKey = vkey->parseVerseList(key.c_str(), "Gen1:1", true);

        bool first = true;
        for (listKey.positionToTop(); !listKey.popError(); listKey.increment()) {
            vkey->positionFrom(listKey);
            if (first) {
                *linkMaster = *vkey;
                std::string text;
                if (!replace)
                    text = module.getRawEntry();
                text += entry;

                std::cout << "adding entry: " << vkey->getText() << " length " << entry.size() << "/" << (unsigned short)text.size() << std::endl;
                module.setEntry(text);
                first = false;
            }
            else {
                std::cout << "linking entry: " << vkey->getText() << " to " << linkMaster->getText() << std::endl;
                module.linkEntry(*linkMaster);
            }
        }
    }
}
