/******************************************************************************
 *  swkey.h    - macros for byteswapping necessary for big endian cpus (or not)
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
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

#ifndef SWBYTESWAP_H
#define SWBYTESWAP_H

#ifdef BIGENDIAN
#ifdef MACOSX

#include <architecture/byte_order.h>
#define SWAP32(x) NXSwapLittleLongToHost(x)
#define SWAP16(x) NXSwapLittleShortToHost(x)

#else
#ifdef SPARC_SOLARIS

#include <sys/pctypes.h>
#define SWAP32(x) lelong(x)
#define SWAP16(x) leshort(x)

#else // all big endian GNU systems

#include <byteswap.h>
#define SWAP32(x) bswap_32(x)
#define SWAP16(x) bswap_16(x)

#endif
#endif
#else //little endian

#define SWAP32(x) x
#define SWAP16(x) x

#endif
#endif

