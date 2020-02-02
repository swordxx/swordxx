/******************************************************************************
 *
 *  rawfiles.cpp -    code for class 'RawFiles'- a module that produces HTML
 *            HREFs pointing to actual text desired.  Uses standard
 *            files:    ot and nt using indexs ??.bks ??.cps ??.vss
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
#include "../../ShareRef.h"
#include "../../sysdata.h"
#include "../../utilstr.h"


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
bool RawFiles::isWritable() const noexcept {
    return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode() & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * RawFiles::getRawEntry    - Retrieve the unprocessed entry contents at
 *                    the current key position of this module
 *
 * RET: entry contents
 */

std::string RawFiles::getRawEntryImpl() const {
    StartType start = 0;
    SizeType size = 0;
    auto const key_(getVerseKey());

    findOffset(key_->getTestament(), key_->getTestamentIndex(), &start, &size);

    if (!size)
        return std::string();
    std::string tmpbuf(m_path);
    tmpbuf += '/';
    tmpbuf += readText(key_->getTestament(), start, size);
    auto const datafile(FileMgr::getSystemFileMgr()->open(tmpbuf.c_str(), FileMgr::RDONLY));
    if (datafile->getFd() > 0) {
        size = datafile->seek(0, SEEK_END);
        std::string r(size + 1, '\0');
        datafile->seek(0, SEEK_SET);
        datafile->read(r.data(), size);
        return r;
//            preptext(entrybuf);
    }
    return std::string();
}


/******************************************************************************
 * RawFiles::setEntry(char *)- Update the module's current key entry with
 *                provided text
 */

void RawFiles::setEntry(std::string_view text) {
    StartType start;
    SizeType size;
    auto const key_(getVerseKey());

    findOffset(key_->getTestament(), key_->getTestamentIndex(), &start, &size);

    std::string filename(m_path.str() + '/');
    if (size) {
        filename += readText(key_->getTestament(), start, size);
    }
    else {
        std::string tmpbuf(getNextFilename());
        doSetText(key_->getTestament(), key_->getTestamentIndex(), tmpbuf);
        filename += tmpbuf;
    }
    auto const datafile(FileMgr::getSystemFileMgr()->open(filename.c_str(), FileMgr::CREAT|FileMgr::WRONLY|FileMgr::TRUNC));
    if (datafile->getFd() > 0)
        datafile->write(text.data(), text.size());
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
    auto const key_(getVerseKey());

    findOffset(key_->getTestament(), key_->getTestamentIndex(), &start, &size);

    if (size) {
        auto const tmpbuf(readText(key_->getTestament(), start, size + 2));
        auto const key2(getVerseKey(shareRef(inkey)));
        doSetText(key2->getTestament(), key2->getTestamentIndex(), tmpbuf);
    }
}


/******************************************************************************
 * RawFiles::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void RawFiles::deleteEntry() {
    auto const key_(getVerseKey());
    using namespace std::literals::string_view_literals;
    doSetText(key_->getTestament(), key_->getTestamentIndex(), ""sv);
}


/******************************************************************************
 * RawFiles::getNextfilename - generates a valid filename in which to store
 *                an entry
 *
 * RET: filename
 */

std::string RawFiles::getNextFilename() {
    uint32_t number = 0;

    auto const fileMgr(FileMgr::getSystemFileMgr());
    std::string r(m_path.str() + "/incfile");
    {
        auto const datafile(fileMgr->open(r.c_str(), FileMgr::RDONLY));
        if (datafile->getFd() != -1) {
            if (datafile->read(&number, 4) != 4) number = 0;
            number = swordtoarch32(number);
        }
    }
    number++;

    r = formatted("%.7d", number-1);
    number = archtosword32(number);
    fileMgr->open(r.c_str(), FileMgr::CREAT|FileMgr::WRONLY|FileMgr::TRUNC)->write(&number, 4);

    return r;
}


char RawFiles::createModule(char const * path) {
    assert(path);
    static auto const zero = swapFromArch(std::uint32_t(0u));

    FileMgr::getSystemFileMgr()->open(
                (std::string(path) + "/incfile").c_str(),
                FileMgr::CREAT|FileMgr::WRONLY|FileMgr::TRUNC)->write(&zero, sizeof(zero));

    return RawVerse::createModule (path);
}



} /* namespace swordxx */
