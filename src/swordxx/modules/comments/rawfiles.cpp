/******************************************************************************
 *
 *  rawfiles.cpp -    code for class 'RawFiles'- a module that produces HTML
 *            HREFs pointing to actual text desired.  Uses standard
 *            files:    ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * $Id$
 *
 * Copyright 1998-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "rawfiles.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../sysdata.h"


namespace swordxx {

 /******************************************************************************
 * RawFiles Constructor - Initializes data for instance of RawFiles
 *
 * ENT:    iname - Internal name for module
 *    idesc - Name to display to user for module
 */

RawFiles::RawFiles(const char *ipath, const char *iname, const char *idesc, TextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : RawVerse(ipath, FileMgr::RDWR), SWCom(iname, idesc, enc, dir, mark, ilang)
{
}


/******************************************************************************
 * RawFiles Destructor - Cleans up instance of RawFiles
 */

RawFiles::~RawFiles()
{
}


/** Is the module writable? :)
* @return yes or no
*/
bool RawFiles::isWritable() const {
    return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * RawFiles::getRawEntry    - Retrieve the unprocessed entry contents at
 *                    the current key position of this module
 *
 * RET: entry contents
 */

std::string &RawFiles::getRawEntryBuf() const {
    FileDesc *datafile;
    StartType start = 0;
    SizeType size = 0;
    VerseKey const & key = getVerseKey();

    findOffset(key.getTestament(), key.getTestamentIndex(), &start, &size);

    entryBuf = "";
    if (size) {
        std::string tmpbuf(m_path);
        tmpbuf += '/';
        readText(key.getTestament(), start, size, entryBuf);
        tmpbuf += entryBuf;
        entryBuf = "";
        datafile = FileMgr::getSystemFileMgr()->open(tmpbuf.c_str(), FileMgr::RDONLY);
        if (datafile->getFd() > 0) {
            size = datafile->seek(0, SEEK_END);
            auto tmpBuf(std::make_unique<char[]>(size + 1));
            std::memset(tmpBuf.get(), 0, size + 1);
            datafile->seek(0, SEEK_SET);
            datafile->read(tmpBuf.get(), size);
            entryBuf = tmpBuf.get();
//            preptext(entrybuf);
        }
        FileMgr::getSystemFileMgr()->close(datafile);
    }
    return entryBuf;
}


/******************************************************************************
 * RawFiles::setEntry(char *)- Update the module's current key entry with
 *                provided text
 */

void RawFiles::setEntry(const char *inbuf, long len) {
    FileDesc *datafile;
    StartType start;
    SizeType size;
    VerseKey const & key = getVerseKey();

    len = (len<0)?std::strlen(inbuf):len;

    findOffset(key.getTestament(), key.getTestamentIndex(), &start, &size);

    if (size) {
        std::string tmpbuf;
        entryBuf = m_path;
        entryBuf += '/';
        readText(key.getTestament(), start, size, tmpbuf);
        entryBuf += tmpbuf;
    }
    else {
        entryBuf = m_path;
        entryBuf += '/';
        std::string tmpbuf(getNextFilename());
        doSetText(key.getTestament(), key.getTestamentIndex(), tmpbuf.c_str());
        entryBuf += tmpbuf;
    }
    datafile = FileMgr::getSystemFileMgr()->open(entryBuf.c_str(), FileMgr::CREAT|FileMgr::WRONLY|FileMgr::TRUNC);
    if (datafile->getFd() > 0) {
        datafile->write(inbuf, len);
    }
    FileMgr::getSystemFileMgr()->close(datafile);
}


/******************************************************************************
 * RawFiles::linkEntry(SWKey *)- Link the modules current key entry with
 *                another module entry
 *
 * RET: *this
 */

void RawFiles::linkEntry(SWKey const & inkey) {
    StartType start;
    SizeType size;
    VerseKey const & key = getVerseKey();

    findOffset(key.getTestament(), key.getTestamentIndex(), &start, &size);

    if (size) {
        std::string tmpbuf;
        readText(key.getTestament(), start, size + 2, tmpbuf);

        VerseKey const & key2 = getVerseKey(&inkey);
        doSetText(key2.getTestament(), key2.getTestamentIndex(), tmpbuf.c_str());
    }
}


/******************************************************************************
 * RawFiles::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void RawFiles::deleteEntry() {
    VerseKey const & key = getVerseKey();
    doSetText(key.getTestament(), key.getTestamentIndex(), "");
}


/******************************************************************************
 * RawFiles::getNextfilename - generates a valid filename in which to store
 *                an entry
 *
 * RET: filename
 */

std::string RawFiles::getNextFilename() {
    uint32_t number = 0;
    FileDesc *datafile;

    std::string r(m_path.str() + "/incfile");
    datafile = FileMgr::getSystemFileMgr()->open(r.c_str(), FileMgr::RDONLY);
    if (datafile->getFd() != -1) {
        if (datafile->read(&number, 4) != 4) number = 0;
        number = swordtoarch32(number);
    }
    number++;
    FileMgr::getSystemFileMgr()->close(datafile);

    datafile = FileMgr::getSystemFileMgr()->open(r.c_str(), FileMgr::CREAT|FileMgr::WRONLY|FileMgr::TRUNC);
    r = formatted("%.7d", number-1);

    number = archtosword32(number);
    datafile->write(&number, 4);

    FileMgr::getSystemFileMgr()->close(datafile);
    return r;
}


char RawFiles::createModule(char const * path) {
    assert(path);
    static auto const zero = swapFromArch(std::uint32_t(0u));

    FileDesc * const datafile =
            FileMgr::getSystemFileMgr()->open(
                (std::string(path) + "/incfile").c_str(),
                FileMgr::CREAT|FileMgr::WRONLY|FileMgr::TRUNC);
    datafile->write(&zero, sizeof(zero));
    FileMgr::getSystemFileMgr()->close(datafile);

    return RawVerse::createModule (path);
}



} /* namespace swordxx */
