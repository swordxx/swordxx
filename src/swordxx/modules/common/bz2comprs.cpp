/******************************************************************************
 *
 *  bz2comprs.cpp -    Bzip2Compress, a driver class that provides bzip2
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

#include "bz2comprs.h"

#include <bzlib.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>


namespace swordxx {

/******************************************************************************
 * Bzip2Compress Constructor - Initializes data for instance of Bzip2Compress
 *
 */

Bzip2Compress::Bzip2Compress() : SWCompress() {
    m_level = 9;
}


/******************************************************************************
 * Bzip2Compress Destructor - Cleans up instance of Bzip2Compress
 */

Bzip2Compress::~Bzip2Compress() {
}


/******************************************************************************
 * Bzip2Compress::Encode - This function "encodes" the input stream into the
 *            output stream.
 *            The GetChars() and SendChars() functions are
 *            used to separate this method from the actual
 *            i/o.
 *         NOTE:    must set zlen for parent class to know length of
 *             compressed buffer.
 */

void Bzip2Compress::Encode(void)
{
    // get buffer
    char chunk[1024];
    std::vector<char> buf(1024, '\0');
    char *chunkbuf = buf.data();
    unsigned long chunklen;
    unsigned long len = 0;
    while((chunklen = GetChars(chunk, 1023, ENCODE))) {
        std::memcpy(chunkbuf, chunk, chunklen);
        len += chunklen;
        if (chunklen < 1023) {
            break;
        } else {
            buf.resize(len + 1024);
        }
        chunkbuf = buf.data() + len;
    }


    m_zlen = (long) (len*1.01)+600;
    auto zbuf(std::make_unique<char[]>(m_zlen + 1));
    if (len)
    {
        //printf("Doing compress\n");
        if (BZ2_bzBuffToBuffCompress(zbuf.get(), (unsigned int*)&m_zlen, buf.data(), len, m_level, 0, 0) != BZ_OK)
        {
            printf("ERROR in compression\n");
        }
        else {
            SendChars(zbuf.get(), m_zlen, ENCODE);
        }
    }
    else
    {
        fprintf(stderr, "ERROR: no buffer to compress\n");
    }
}


/******************************************************************************
 * Bzip2Compress::Decode - This function "decodes" the input stream into the
 *            output stream.
 *            The GetChars() and SendChars() functions are
 *            used to separate this method from the actual
 *            i/o.
 */

void Bzip2Compress::Decode(void)
{
    // get buffer
    char chunk[1024];
    std::vector<char> zbuf(1024u, '\0');
    char *chunkbuf = zbuf.data();
    int chunklen;
    unsigned long zlen = 0;
    while((chunklen = GetChars(chunk, 1023, DECODE))) {
        std::memcpy(chunkbuf, chunk, chunklen);
        zlen += chunklen;
        if (chunklen < 1023) {
            break;
        } else {
            zbuf.resize(zlen + 1024);
        }
        chunkbuf = zbuf.data() + zlen;
    }

    //printf("Decoding complength{%ld} uncomp{%ld}\n", zlen, blen);
    if (zlen) {
        unsigned int blen = zlen*20;    // trust compression is less than 1000%
        auto buf(std::make_unique<char[]>(blen));
        //printf("Doing decompress {%s}\n", zbuf);
        m_slen = 0;
        switch (BZ2_bzBuffToBuffDecompress(buf.get(), &blen, zbuf.data(), zlen, 0, 0)){
            case BZ_OK: SendChars(buf.get(), blen, DECODE); m_slen = blen; break;
            case BZ_MEM_ERROR: fprintf(stderr, "ERROR: not enough memory during decompression.\n"); break;
            case BZ_OUTBUFF_FULL: fprintf(stderr, "ERROR: not enough room in the out buffer during decompression.\n"); break;
            case BZ_DATA_ERROR: fprintf(stderr, "ERROR: corrupt data during decompression.\n"); break;
            default: fprintf(stderr, "ERROR: an unknown error occurred during decompression.\n"); break;
        }
    }
    else {
        fprintf(stderr, "ERROR: no buffer to decompress!\n");
    }
    //printf("Finished decoding\n");
}

} /* namespace swordxx */
