/******************************************************************************
 *
 *  swcomprs.h -    definition of Class SWCompress used for data
 *            compression
 *
 * $Id$
 *
 * Copyright 1996-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWCOMPRS_H
#define SWCOMPRS_H

#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT SWCompress {

public: /* Types: */

    enum Direction { ENCODE, DECODE };

public: /* Methods: */

    SWCompress();
    virtual ~ SWCompress();
    virtual char * Buf(char const * buf = nullptr, unsigned long * len = nullptr);
    virtual char * zBuf(unsigned long * len, char * buf = nullptr);
    virtual unsigned long GetChars(char * buf, unsigned long len, Direction dir);
    virtual unsigned long SendChars(char * buf, unsigned long len, Direction dir);
    virtual void Encode();
    virtual void Decode();
    virtual void setLevel(int l) { level = l; }
    virtual int getLevel() { return level; }

private: /* Methods: */

    void reset() noexcept;
    void cycleStream(Direction dir);

protected: /* Fields: */

    char * buf = nullptr;
    char * zbuf = nullptr;
    unsigned long zlen = 0;
    unsigned long zpos = 0;
    unsigned long pos = 0;
    unsigned long slen = 0;
    int level = 6;

};

} /* namespace swordxx */
#endif
