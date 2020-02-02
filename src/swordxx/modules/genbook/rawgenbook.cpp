/******************************************************************************
 *
 *  rawgenbook.cpp -    code for class 'RawGenBook'- a module that reads raw
 *            text files: ot and nt using indexs ??.bks ??.cps ??.vss
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
#include <cstring>
#include <fcntl.h>
#include <memory>
#include "../../filemgr.h"
#include "../../keys/treekeyidx.h"
#include "../../keys/versetreekey.h"
#include "../../utilstr.h"
#include "../../ShareRef.h"
#include "../../sysdata.h"
#include "../common/rawstr.h"


namespace swordxx {

namespace {

std::shared_ptr<SWKey> staticCreateKey(NormalizedPath const & path, bool const verseKey) {
    auto tKey(std::make_shared<TreeKeyIdx>(path.c_str()));
    if (!verseKey)
        return tKey;
    auto vtKey(std::make_shared<VerseTreeKey>(*tKey));
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

RawGenBook::~RawGenBook() = default;


bool RawGenBook::isWritable() const noexcept {
    return ((bdtfd->getFd() > 0) && ((bdtfd->mode() & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * RawGenBook::getRawEntry    - Returns the correct verse when char * cast
 *                    is requested
 *
 * RET: string buffer with verse
 */

std::string RawGenBook::getRawEntryImpl() const {

    uint32_t offset = 0;
    uint32_t size = 0;

    auto const key_(getTreeKey());

    int dsize;
    key_->getUserData(&dsize);
    std::string entry;
    if (dsize > 7) {
        std::memcpy(&offset, key_->getUserData(), 4);
        offset = swordtoarch32(offset);

        std::memcpy(&size, key_->getUserData() + 4, 4);
        size = swordtoarch32(size);

        entry.resize(size, '\0');
        bdtfd->seek(offset, SEEK_SET);
        bdtfd->read(&entry[0u], size);

        rawFilter(entry, nullptr);    // hack, decipher
        rawFilter(entry, key_.get());
    }

    return entry;
}


void RawGenBook::setEntry(std::string_view text) {
    uint32_t offset = archtosword32(bdtfd->seek(0, SEEK_END));
    auto const tk(getTreeKey());
    /// \bug remove const_cast:
    auto const key_ = static_cast<TreeKeyIdx *>(const_cast<TreeKey *>(tk.get()));

    char userData[8];

    bdtfd->write(text.data(), text.size());

    std::uint32_t size = text.size();
    size = archtosword32(size);
    std::memcpy(userData, &offset, 4);
    std::memcpy(userData+4, &size, 4);
    key_->setUserData(userData, 8);
    key_->save();
}


void RawGenBook::linkEntry(SWKey const & inkey) {
    /// \bug Remove const_cast:
    TreeKeyIdx * key_ =
            const_cast<TreeKeyIdx *>(
                static_cast<TreeKeyIdx const *>(getTreeKey().get()));
    // see if we have a VerseKey * or decendant
    /// \bug Remove const_cast:
    TreeKeyIdx * srckey =
            const_cast<TreeKeyIdx *>(dynamic_cast<TreeKeyIdx const *>(&inkey));
    // if we don't have a VerseKey * decendant, create our own
    if (!srckey) {
        auto const tmp(createKey());
        srckey = &static_cast<TreeKeyIdx &>(*tmp);
        srckey->positionFrom(inkey);
    }

    key_->setUserData(srckey->getUserData(), 8);
    key_->save();
}


/******************************************************************************
 * RawGenBook::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void RawGenBook::deleteEntry() {
    /// \todo Remove const_cast:
    const_cast<TreeKeyIdx *>(static_cast<TreeKeyIdx const *>(getTreeKey().get()))->remove();
}


char RawGenBook::createModule(NormalizedPath const & path) {
    signed char retval;

    auto const buf(path.str() + ".bdt");
    FileMgr::removeFile(buf.c_str());
    FileMgr::getSystemFileMgr()->touch(buf.c_str());

    retval = TreeKeyIdx::create(path.c_str());
    return retval;
}

std::shared_ptr<SWKey> RawGenBook::createKey() const
{ return staticCreateKey(m_path.c_str(), verseKey); }

bool RawGenBook::hasEntry(SWKey const & k) const {
    auto const key_(getTreeKey(shareRef(k)));

    int dsize;
    key_->getUserData(&dsize);
    return (dsize > 7) && key_->getError() == 0;
}

} /* namespace swordxx */
