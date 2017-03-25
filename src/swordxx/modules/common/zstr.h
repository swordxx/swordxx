/*****************************************************************************
 *
 *  zstr.h -    code for class 'zStr'- a module that reads compressed text
 *               files.
 *        and provides lookup and parsing functions based on
 *        class StrKey
 *
 * $Id$
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

#ifndef ZSTR_H
#define ZSTR_H

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
    mutable EntriesBlock *cacheBlock;
    mutable long cacheBlockIndex;
    mutable bool cacheDirty;
    bool caseSensitive;
    mutable long lastoff;        // for caching and optimization
    long blockCount;
    std::unique_ptr<SWCompress> const compressor;

protected:
    FileDesc *idxfd;
    FileDesc *datfd;
    FileDesc *zdxfd;
    FileDesc *zdtfd;
    static const int IDXENTRYSIZE;
    static const int ZDXENTRYSIZE;

    void getCompressedText(long block, long entry, char **buf) const;
    void flushCache() const;
    void getKeyFromDatOffset(long ioffset, char **buf) const;
    void getKeyFromIdxOffset(long ioffset, char **buf) const;

public:
    zStr(char const * ipath,
         int fileMode = -1,
         long blockCount = 100,
         std::unique_ptr<SWCompress> icomp = nullptr,
         bool caseSensitive = false);
    virtual ~zStr();
    signed char findKeyIndex(const char *ikey, long *idxoff, long away = 0) const;
    void getText(long index, char **idxbuf, char **buf) const;
    void setText(const char *ikey, const char *buf, long len = -1);
    void linkEntry(const char *destkey, const char *srckey);
    virtual void rawZFilter(std::string &buf, char direction = 0) const { (void) buf; (void) direction; }
    static signed char createModule (const char *path);
};

} /* namespace swordxx */
#endif
