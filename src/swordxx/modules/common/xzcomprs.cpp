/******************************************************************************
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

#include "xzcomprs.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#define LZMA_API_STATIC
#include <lzma.h>
#include <memory>
#include <vector>


namespace swordxx {

/******************************************************************************
 * XzCompress Constructor - Initializes data for instance of XzCompress
 *
 */

XzCompress::XzCompress()
    : SWCompress()
{ setLevel(3); }


/******************************************************************************
 * XzCompress Destructor - Cleans up instance of XzCompress
 */

XzCompress::~XzCompress() {
}


/******************************************************************************
 * XzCompress::Encode - This function "encodes" the input stream into the
 *            output stream.
 *            The GetChars() and SendChars() functions are
 *            used to separate this method from the actual
 *            i/o.
 *         NOTE:    must set zlen for parent class to know length of
 *             compressed buffer.
 */

void XzCompress::Encode(void)
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

    m_zlen = (long)lzma_stream_buffer_bound(len);
    auto zbuf(std::make_unique<char[]>(m_zlen + 1));
    std::size_t zpos = 0;

    if (len)
    {
        //printf("Doing compress\n");
        switch (lzma_easy_buffer_encode(m_level | LZMA_PRESET_EXTREME, LZMA_CHECK_CRC64, nullptr, (const uint8_t*)buf.data(), (std::size_t)len, (uint8_t*)zbuf.get(), &zpos, (std::size_t)m_zlen)) {
                case LZMA_OK: SendChars(zbuf.get(), zpos, ENCODE);  break;
            case LZMA_BUF_ERROR: fprintf(stderr, "ERROR: not enough room in the out buffer during compression.\n"); break;
            case LZMA_UNSUPPORTED_CHECK: fprintf(stderr, "ERROR: unsupported_check error encountered during decompression.\n"); break;
            case LZMA_OPTIONS_ERROR: fprintf(stderr, "ERROR: options error encountered during decompression.\n"); break;
            case LZMA_MEM_ERROR: fprintf(stderr, "ERROR: not enough memory during compression.\n"); break;
            case LZMA_DATA_ERROR: fprintf(stderr, "ERROR: corrupt data during compression.\n"); break;
            case LZMA_PROG_ERROR: fprintf(stderr, "ERROR: program error encountered during decompression.\n"); break;
            default: fprintf(stderr, "ERROR: an unknown error occurred during compression.\n"); break;
        }
    }
    else
    {
        fprintf(stderr, "ERROR: no buffer to compress\n");
    }
}


/******************************************************************************
 * XzCompress::Decode - This function "decodes" the input stream into the
 *            output stream.
 *            The GetChars() and SendChars() functions are
 *            used to separate this method from the actual
 *            i/o.
 */

void XzCompress::Decode(void)
{
    // get buffer
    char chunk[1024];
    std::vector<char> zbuf(1024, '\0');
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
        unsigned long blen = zlen*20;    // trust compression is less than 2000%
        auto buf(std::make_unique<char[]>(blen));
        //printf("Doing decompress {%s}\n", zbuf);
        m_slen = 0;
        std::size_t zpos = 0;
        std::size_t bpos = 0;

        switch (lzma_stream_buffer_decode((uint64_t *)&memlimit, 0, nullptr, (const uint8_t*)zbuf.data(), &zpos, (std::size_t)zlen, (uint8_t*)buf.get(), &bpos, (std::size_t)&blen)){
            case LZMA_OK: SendChars(buf.get(), bpos, DECODE); m_slen = bpos; break;
            case LZMA_FORMAT_ERROR: fprintf(stderr, "ERROR: format error encountered during decompression.\n"); break;
            case LZMA_OPTIONS_ERROR: fprintf(stderr, "ERROR: options error encountered during decompression.\n"); break;
            case LZMA_DATA_ERROR: fprintf(stderr, "ERROR: corrupt data during decompression.\n"); break;
            case LZMA_NO_CHECK: fprintf(stderr, "ERROR: no_check error encountered during decompression.\n"); break;
            case LZMA_UNSUPPORTED_CHECK: fprintf(stderr, "ERROR: unsupported_check error encountered during decompression.\n"); break;
            case LZMA_MEMLIMIT_ERROR: fprintf(stderr, "ERROR: memlimit error encountered during decompression.\n"); break;
            case LZMA_MEM_ERROR: fprintf(stderr, "ERROR: not enough memory during decompression.\n"); break;
            case LZMA_BUF_ERROR: fprintf(stderr, "ERROR: not enough room in the out buffer during decompression.\n"); break;
            case LZMA_PROG_ERROR: fprintf(stderr, "ERROR: program error encountered during decompression.\n"); break;
            default: fprintf(stderr, "ERROR: an unknown error occurred during decompression.\n"); break;
        }
    }
    else {
        fprintf(stderr, "ERROR: no buffer to decompress!\n");
    }
    //printf("Finished decoding\n");
}


/******************************************************************************
 * XzCompress::SetLevel - This function sets the compression level of the
 *            compressor.
 */

void XzCompress::setLevel(int l) {
    m_level = l;

    // having changed the compression level, we need to adjust our memlimit accordingly,
    // as in the constructor:

    // start with the estimated memory usage for our preset
    memlimit = lzma_easy_decoder_memusage(m_level | LZMA_PRESET_EXTREME);

    // and round up to a power of 2--
    // bit twiddle hack to determine next greatest power of 2 from:
    // http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    memlimit--;
    memlimit |= memlimit >> 1;
    memlimit |= memlimit >> 2;
    memlimit |= memlimit >> 4;
    memlimit |= memlimit >> 8;
    memlimit |= memlimit >> 16;
    memlimit++;

    // double that for safety's sake
    memlimit <<= 1;
};

} /* namespace swordxx */
