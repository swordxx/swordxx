/******************************************************************************
 *
 *  bz2comprs.h -    Bzip2Compress, a driver class that provides bzip2
 *            compression (Burrows–Wheeler with Huffman coding)
 *
 * Copyright 2000-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_BZ2COMPRS_H
#define SWORDXX_BZ2COMPRS_H

#include "swcomprs.h"

#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT Bzip2Compress : public SWCompress {

protected:
public:
    Bzip2Compress();
    ~Bzip2Compress() override;

    void Encode() override;
    void Decode() override;
};

} /* namespace swordxx */

#endif /* SWORDXX_BZ2COMPRS_H */
