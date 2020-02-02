/*****************************************************************************
 *
 *  zstr.h -    code for class 'zStr'- a module that reads compressed text
 *               files.
 *        and provides lookup and parsing functions based on
 *        class StrKey
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_ZSTR_H
#define SWORDXX_ZSTR_H

#include <string>
#include "../../defs.h"

#include <memory>
#include "swcomprs.h"


namespace swordxx {

class SWCompress;
class EntriesBlock;
class FileDesc;

class SWDLLEXPORT zStr {

private:
    mutable std::unique_ptr<EntriesBlock> m_cacheBlock;
    mutable long m_cacheBlockIndex;
    mutable bool m_cacheDirty;
    bool m_caseSensitive;
    mutable long m_lastoff;        // for caching and optimization
    long m_blockCount;
    std::unique_ptr<SWCompress> const m_compressor;

protected:
    std::shared_ptr<FileDesc> idxfd;
    std::shared_ptr<FileDesc> datfd;
    std::shared_ptr<FileDesc> zdxfd;
    std::shared_ptr<FileDesc> zdtfd;
    static const int IDXENTRYSIZE;
    static const int ZDXENTRYSIZE;

    void getCompressedText(long block, long entry, std::string & buf) const;
    void flushCache() const;
    std::string getKeyFromDatOffset(long ioffset) const;
    std::string getKeyFromIdxOffset(long ioffset) const;

public:
    zStr(char const * ipath,
         int fileMode = -1,
         long m_blockCount = 100,
         std::unique_ptr<SWCompress> icomp = nullptr,
         bool m_caseSensitive = false);
    virtual ~zStr();
    signed char findKeyIndex(const char *ikey, long *idxoff, long away = 0) const;
    void getText(long index, std::string & idxbuf, std::string & buf) const;
    void setText(const char *ikey, std::string_view text);
    void linkEntry(const char *destkey, const char *srckey);
    virtual void rawZFilter(std::string &buf, char direction = 0) const;
    static signed char createModule (const char *path);
};

} /* namespace swordxx */

#endif /* SWORDXX_ZSTR_H */
