/*
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
 *      CrossWire Bible Society
 *      P. O. Box 2528
 *      Tempe, AZ  85280-2528
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

#include "swunicod.h"
SWORD_NAMESPACE_START
unsigned char* UTF32to8 (unsigned long utf32, unsigned char * utf8) {
  unsigned int i;
  for (i = 0; i < 6; i++) utf8[i] = 0;

  if (utf32 < 0x80) {
    utf8[0] = (char)utf32;
  }
  else if (utf32 < 0x800) {
    i = utf32 & 0x3f;
    utf8[1] = 0x80 | i;
    utf32 >>= 6;
   
    i = utf32 & 0x1f;
    utf8[0] = 0xc0 | i;
  }
  else if (utf32 < 0x10000) {
    i = utf32 & 0x3f;
    utf8[2] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[1] = 0x80 | i;
    utf32 >>= 6;
   
    i = utf32 & 0x0f;
    utf8[0] = 0xe0 | i;
  }
  else if (utf32 < 0x200000) {
    i = utf32 & 0x3f;
    utf8[3] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[2] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[1] = 0x80 | i;
    utf32 >>= 6;
   
    i = utf32 & 0x07;
    utf8[0] = 0xf0 | i;
  }
  else if (utf32 < 0x4000000) {
    i = utf32 & 0x3f;
    utf8[4] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[3] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[2] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[1] = 0x80 | i;
    utf32 >>= 6;
   
    i = utf32 & 0x03;
    utf8[0] = 0xf8 | i;
  }
  else if (utf32 < 0x80000000) {
    i = utf32 & 0x3f;
    utf8[5] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[4] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[3] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[2] = 0x80 | i;
    utf32 >>= 6;

    i = utf32 & 0x3f;
    utf8[1] = 0x80 | i;
    utf32 >>= 6;
   
    i = utf32 & 0x01;
    utf8[0] = 0xfc | i;
  }
  return utf8;
}

unsigned long UTF8to32 (unsigned char * utf8) {

  unsigned char i = utf8[0];
  unsigned char count;
  unsigned long utf32 = 0;

  for (count = 0; i & 0x80; count++) i <<= 1;
  if (!count) {
    return utf8[0];
  }
  else if (count == 1) {
    return 0xffff;
  }
  else {
    count--;
    utf32 = i >> count;
    for (i = 1; i <= count; i++) {
      if ((0xc0 & utf8[i]) != 0x80) {
	return  0xffff;
      }
      utf32 <<= 6;
      utf32 |= (utf8[i] & 0x3f);
    }
  }
  return utf32;
}

SWORD_NAMESPACE_END
