/******************************************************************************
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

#ifndef SWORDXX_SWCOMPRS_H
#define SWORDXX_SWCOMPRS_H

#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT SWCompress {

public: /* Types: */

    enum Direction { ENCODE, DECODE };

public: /* Methods: */

    SWCompress();
    virtual ~ SWCompress();
    char * Buf(char const * m_buf = nullptr, unsigned long * len = nullptr);
    char * zBuf(unsigned long * len, char * m_buf = nullptr);
    virtual unsigned long GetChars(char * m_buf, unsigned long len, Direction dir);
    virtual unsigned long SendChars(char * m_buf, unsigned long len, Direction dir);
    virtual void Encode();
    virtual void Decode();
    virtual void setLevel(int l) { m_level = l; }
    virtual int getLevel() { return m_level; }

private: /* Methods: */

    void reset() noexcept;
    void cycleStream(Direction dir);

protected: /* Fields: */

    char * m_buf = nullptr;
    char * m_zbuf = nullptr;
    unsigned long m_zlen = 0;
    unsigned long m_zpos = 0;
    unsigned long m_pos = 0;
    unsigned long m_slen = 0;
    int m_level = 6;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWCOMPRS_H */
