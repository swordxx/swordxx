/******************************************************************************
 *
 *  sysdata.h -	
 *
 * $Id$
 * 
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SIZEDTYPES_H
#define SIZEDTYPES_H

#undef __swswap16
#undef __swswap32
#undef __swswap64

#define __swswap16(x) \
	((uint16_t)( \
		(((uint16_t)(x) & (uint16_t)0x00ffU) << 8) | \
		(((uint16_t)(x) & (uint16_t)0xff00U) >> 8) ))

		
#define __swswap32(x) \
	((uint32_t)( \
		(((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
		(((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) | \
		(((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) | \
		(((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24) ))

		
#define __swswap64(x) \
	((uint64_t)( \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) | \
		   (uint64_t)(((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56) ))
		



#ifndef WORDS_BIGENDIAN

#define swordtoarch16(x) (x)
#define swordtoarch32(x) (x)
#define swordtoarch64(x) (x)
#define archtosword16(x) (x)
#define archtosword32(x) (x)
#define archtosword64(x) (x)

#else 

#define swordtoarch16(x) __swswap16(x)
#define swordtoarch32(x) __swswap32(x)
#define swordtoarch64(x) __swswap64(x)
#define archtosword16(x) __swswap16(x)
#define archtosword32(x) __swswap32(x)
#define archtosword64(x) __swswap64(x)


#endif


#endif
