/******************************************************************************
 *
 *  rawgenbook.cpp -    code for class 'RawGenBook'- a module that reads raw
 *            text files: ot and nt using indexs ??.bks ??.cps ??.vss
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

#include "rawgenbook.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <memory>
#include "../../filemgr.h"
#include "../../keys/treekeyidx.h"
#include "../../keys/versetreekey.h"
#include "../../utilstr.h"
#include "../../sysdata.h"
#include "../common/rawstr.h"


namespace swordxx {

namespace {

std::unique_ptr<SWKey> staticCreateKey(NormalizedPath const & path, bool const verseKey) {
    auto tKey(std::make_unique<TreeKeyIdx>(path.c_str()));
    if (!verseKey)
        return tKey;
    auto vtKey(std::make_unique<VerseTreeKey>(*tKey));
    tKey.reset();
    return vtKey;
}

} // anonymous namespace


/******************************************************************************
 * RawGenBook Constructor - Initializes data for instance of RawGenBook
 *
 * ENT:    iname - Internal name for module
 *    idesc - Name to display to user for module
 */

RawGenBook::RawGenBook(NormalizedPath const & path, const char *iname, const char *idesc, TextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang, const char *keyType)
    : SWGenBook(staticCreateKey(path, !std::strcmp("VerseKey", keyType)), iname, idesc, enc, dir, mark, ilang)
    , m_path(std::move(path))
{
    verseKey = !std::strcmp("VerseKey", keyType);

    if (verseKey) setType("Biblical Texts");

    bdtfd = FileMgr::getSystemFileMgr()->open((m_path + ".bdt").c_str(), FileMgr::RDWR, true);
}


/******************************************************************************
 * RawGenBook Destructor - Cleans up instance of RawGenBook
 */

RawGenBook::~RawGenBook() {
    FileMgr::getSystemFileMgr()->close(bdtfd);
}


bool RawGenBook::isWritable() const {
    return ((bdtfd->getFd() > 0) && ((bdtfd->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * RawGenBook::getRawEntry    - Returns the correct verse when char * cast
 *                    is requested
 *
 * RET: string buffer with verse
 */

std::string &RawGenBook::getRawEntryBuf() const {

    uint32_t offset = 0;
    uint32_t size = 0;

    const TreeKey &key = getTreeKey();

    int dsize;
    key.getUserData(&dsize);
    entryBuf = "";
    if (dsize > 7) {
        std::memcpy(&offset, key.getUserData(), 4);
        offset = swordtoarch32(offset);

        std::memcpy(&size, key.getUserData() + 4, 4);
        size = swordtoarch32(size);

        entrySize = size;        // support getEntrySize call

        entryBuf.resize(size, '\0');
        bdtfd->seek(offset, SEEK_SET);
        bdtfd->read(&entryBuf[0u], size);

        rawFilter(entryBuf, nullptr);    // hack, decipher
        rawFilter(entryBuf, &key);

//           if (!isUnicode())
            SWModule::prepText(entryBuf);
    }

    return entryBuf;
}


void RawGenBook::setEntry(const char *inbuf, long len) {

    uint32_t offset = archtosword32(bdtfd->seek(0, SEEK_END));
    uint32_t size = 0;
    TreeKeyIdx *key = ((TreeKeyIdx *)&(getTreeKey()));

    char userData[8];

    if (len < 0)
        len = std::strlen(inbuf);

    bdtfd->write(inbuf, len);

    size = archtosword32(len);
    std::memcpy(userData, &offset, 4);
    std::memcpy(userData+4, &size, 4);
    key->setUserData(userData, 8);
    key->save();
}


void RawGenBook::linkEntry(SWKey const & inkey) {
    TreeKeyIdx *key = ((TreeKeyIdx *)&(getTreeKey()));
    // see if we have a VerseKey * or decendant
    /// \bug Remove const_cast:
    TreeKeyIdx * srckey =
            const_cast<TreeKeyIdx *>(dynamic_cast<TreeKeyIdx const *>(&inkey));
    // if we don't have a VerseKey * decendant, create our own
    if (!srckey) {
        srckey = static_cast<TreeKeyIdx *>(createKey().release());
        srckey->positionFrom(inkey);
    }

    key->setUserData(srckey->getUserData(), 8);
    key->save();

    if (&inkey != srckey) // free our key if we created a VerseKey
        delete srckey;
}


/******************************************************************************
 * RawGenBook::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void RawGenBook::deleteEntry() {
    TreeKeyIdx *key = ((TreeKeyIdx *)&(getTreeKey()));
    key->remove();
}


char RawGenBook::createModule(NormalizedPath const & path) {
    FileDesc *fd;
    signed char retval;

    auto const buf(path.str() + ".bdt");
    FileMgr::removeFile(buf.c_str());
    fd = FileMgr::getSystemFileMgr()->open(buf.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd->getFd();
    FileMgr::getSystemFileMgr()->close(fd);

    retval = TreeKeyIdx::create(path.c_str());
    return retval;
}

std::unique_ptr<SWKey> RawGenBook::createKey() const
{ return staticCreateKey(m_path.c_str(), verseKey); }

bool RawGenBook::hasEntry(const SWKey *k) const {
    /// \bug remove const_cast:
    TreeKey &key = getTreeKey(const_cast<SWKey *>(k));

    int dsize;
    key.getUserData(&dsize);
    return (dsize > 7) && key.popError() == '\x00';
}

} /* namespace swordxx */
