/******************************************************************************
 *
 *  xzcomprs.h -    XzCompress, a driver class that provides xz (LZMA2)
 *            compression
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

#ifndef SWORDXX_XZCOMPRS_H
#define SWORDXX_XZCOMPRS_H

#include "swcomprs.h"

#include <cstdint>
#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT XzCompress : public SWCompress {

protected:
public:
    XzCompress();
    ~XzCompress() override;

    void Encode() override;
    void Decode() override;
    void setLevel(int l) override;
private:
    uint64_t memlimit; // memory usage limit during decompression
};

} /* namespace swordxx */

#endif /* SWORDXX_XZCOMPRS_H */
