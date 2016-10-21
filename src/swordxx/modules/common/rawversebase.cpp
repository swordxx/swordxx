/******************************************************************************
 *
 *  rawverse.cpp -    code for class 'RawVerse'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "rawversebase.h"

#include <cassert>
#include "../../filemgr.h"
#include "../../utilstr.h"


namespace swordxx {

/******************************************************************************
 * RawVerse Constructor - Initializes data for instance of RawVerse
 *
 * ENT:    ipath - path of the directory where data and index files are located.
 *        be sure to include the trailing separator (e.g. '/' or '\')
 *        (e.g. 'modules/texts/rawtext/webster/')
 */
RawVerseBase::RawVerseBase(char const * ipath_, int fileMode) {
    assert(ipath_);
    std::string ipath(ipath_);
    removeTrailingDirectorySlashes(ipath);

    std::string buf(ipath + "/ot.vss");

    path = nullptr;
    stdstr(&path, ipath.c_str());

    if (fileMode == -1) { // try read/write if possible
        fileMode = FileMgr::RDWR;
    }

    auto const it(buf.rbegin() + 5u);
    idxfp[0] = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);

    (*it) = 'n';
    idxfp[1] = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);

    buf.resize(buf.size() - 4u);
    textfp[1] = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);

    (*it) = 'o';
    textfp[0] = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);
}

RawVerseBase::~RawVerseBase() noexcept {
    delete[] path;
    FileMgr::getSystemFileMgr()->close(idxfp[0u]);
    FileMgr::getSystemFileMgr()->close(idxfp[1u]);
    FileMgr::getSystemFileMgr()->close(textfp[0u]);
    FileMgr::getSystemFileMgr()->close(textfp[1u]);
}

/******************************************************************************
 * RawVerseBase::readtext    - gets text at a given offset
 *
 * ENT:    testmt    - testament file to search in (0 - Old; 1 - New)
 *    start    - starting offset where the text is located in the file
 *    size    - size of text entry + 2 (null)(null)
 *    buf    - buffer to store text
 *
 */

void RawVerseBase::readText(char testmt,
                            long start,
                            unsigned short size,
                            std::string & buf) const
{
    buf.clear();
    buf.resize(size + 1u, '\0');
    if (!testmt)
        testmt = ((idxfp[1]) ? 1:2);
    if (size) {
        if (textfp[testmt-1]->getFd() >= 0) {
            textfp[testmt-1]->seek(start, SEEK_SET);
            textfp[testmt-1]->read(&buf[0u], (int)size);
        }
    }
}

} /* namespace swordxx */
