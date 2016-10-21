/******************************************************************************
 *
 *  rawverse4.cpp -    code for class 'RawVerse4'- a module that reads raw
 *            text files:
 *            ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey
 *
 * $Id$
 *
 * Copyright 2007-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "rawverse4.h"

#include <cassert>
#include <cctype>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../sysdata.h"
#include "../../utilstr.h"


namespace swordxx {

/******************************************************************************
 * RawVerse4 Statics
 */

const char RawVerse4::nl = '\n';


/******************************************************************************
 * RawVerse4 Constructor - Initializes data for instance of RawVerse4
 *
 * ENT:    ipath - path of the directory where data and index files are located.
 *        be sure to include the trailing separator (e.g. '/' or '\')
 *        (e.g. 'modules/texts/rawtext/webster/')
 */

RawVerse4::RawVerse4(char const * ipath_, int fileMode) {
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


/******************************************************************************
 * RawVerse4 Destructor - Cleans up instance of RawVerse4
 */

RawVerse4::~RawVerse4()
{
    int loop1;

    delete[] path;

    for (loop1 = 0; loop1 < 2; loop1++) {
        FileMgr::getSystemFileMgr()->close(idxfp[loop1]);
        FileMgr::getSystemFileMgr()->close(textfp[loop1]);
    }
}


/******************************************************************************
 * RawVerse4::findoffset    - Finds the offset of the key verse from the indexes
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    idxoff    - offset into .vss
 *    start    - address to store the starting offset
 *    size    - address to store the size of the entry
 */

void RawVerse4::findOffset(char testmt, long idxoff, long *start, unsigned long *size) const {
    idxoff *= 8;
    if (!testmt)
        testmt = ((idxfp[1]) ? 1:2);

    if (idxfp[testmt-1]->getFd() >= 0) {
        idxfp[testmt-1]->seek(idxoff, SEEK_SET);
        uint32_t tmpStart;
        uint32_t tmpSize;
        idxfp[testmt-1]->read(&tmpStart, 4);
        long len = idxfp[testmt-1]->read(&tmpSize, 4);         // read size

        *start = swordtoarch32(tmpStart);
        *size  = swordtoarch32(tmpSize);

        if (len < 4) {
            *size = (unsigned long)((*start) ? (textfp[testmt-1]->seek(0, SEEK_END) - (long)*start) : 0);    // if for some reason we get an error reading size, make size to end of file
        }
    }
    else {
        *start = 0;
        *size = 0;
    }
}


/******************************************************************************
 * RawVerse4::readtext    - gets text at a given offset
 *
 * ENT:    testmt    - testament file to search in (0 - Old; 1 - New)
 *    start    - starting offset where the text is located in the file
 *    size    - size of text entry + 2 (null)(null)
 *    buf    - buffer to store text
 *
 */

void RawVerse4::readText(char testmt, long start, unsigned long size, std::string &buf) const {
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


/******************************************************************************
 * RawVerse4::settext    - Sets text for current offset
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    idxoff    - offset into .vss
 *    buf    - buffer to store
 *      len     - length of buffer (0 - null terminated)
 */

void RawVerse4::doSetText(char testmt, long idxoff, const char *buf, long len)
{
    uint32_t start;
    uint32_t size;

    idxoff *= 8;
    if (!testmt)
        testmt = ((idxfp[1]) ? 1:2);

    size = (len < 0) ? strlen(buf) : len;

    start = textfp[testmt-1]->seek(0, SEEK_END);
    idxfp[testmt-1]->seek(idxoff, SEEK_SET);

    if (size) {
        textfp[testmt-1]->seek(start, SEEK_SET);
        textfp[testmt-1]->write(buf, (int)size);

        // add a new line to make data file easier to read in an editor
        textfp[testmt-1]->write(&nl, 1);
    }
    else {
        start = 0;
    }

    start = archtosword32(start);
    size  = archtosword32(size);

    idxfp[testmt-1]->write(&start, 4);
    idxfp[testmt-1]->write(&size, 4);
}


/******************************************************************************
 * RawVerse4::linkentry    - links one entry to another
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    destidxoff    - dest offset into .vss
 *    srcidxoff        - source offset into .vss
 */

void RawVerse4::doLinkEntry(char testmt, long destidxoff, long srcidxoff) {
    uint32_t start;
    uint32_t size;

    destidxoff *= 8;
    srcidxoff  *= 8;

    if (!testmt)
        testmt = ((idxfp[1]) ? 1:2);

    // get source
    idxfp[testmt-1]->seek(srcidxoff, SEEK_SET);
    idxfp[testmt-1]->read(&start, 4);
    idxfp[testmt-1]->read(&size, 4);

    // write dest
    idxfp[testmt-1]->seek(destidxoff, SEEK_SET);
    idxfp[testmt-1]->write(&start, 4);
    idxfp[testmt-1]->write(&size, 4);
}


/******************************************************************************
 * RawVerse4::CreateModule    - Creates new module files
 *
 * ENT: path    - directory to store module files
 * RET: error status
 */

char RawVerse4::createModule(const char *ipath, const char *v11n)
{
    char * path = nullptr;
    char *buf = new char [ strlen (ipath) + 20 ];
    FileDesc *fd, *fd2;

    stdstr(&path, ipath);

    if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
        path[strlen(path)-1] = 0;

    sprintf(buf, "%s/ot", path);
    FileMgr::removeFile(buf);
    fd = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd->getFd();
    FileMgr::getSystemFileMgr()->close(fd);

    sprintf(buf, "%s/nt", path);
    FileMgr::removeFile(buf);
    fd = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd->getFd();
    FileMgr::getSystemFileMgr()->close(fd);

    sprintf(buf, "%s/ot.vss", path);
    FileMgr::removeFile(buf);
    fd = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd->getFd();

    sprintf(buf, "%s/nt.vss", path);
    FileMgr::removeFile(buf);
    fd2 = FileMgr::getSystemFileMgr()->open(buf, FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd2->getFd();

    VerseKey vk;
    vk.setVersificationSystem(v11n);
    vk.setIntros(1);
    uint32_t offset = 0;
    uint32_t size = 0;
    offset = archtosword32(offset);
    size   = archtosword32(size);

    for (vk = Position::Top; !vk.popError(); ++vk) {
        if (vk.getTestament() < 2) {
            fd->write(&offset, 4);
            fd->write(&size, 4);
        }
        else    {
            fd2->write(&offset, 4);
            fd2->write(&size, 4);
        }
    }
    fd2->write(&offset, 4);
    fd2->write(&size, 4);

    FileMgr::getSystemFileMgr()->close(fd);
    FileMgr::getSystemFileMgr()->close(fd2);

    delete [] path;
    delete [] buf;

    return 0;
}

} /* namespace swordxx */
