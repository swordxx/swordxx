/******************************************************************************
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

#include <cstring>
#include <iostream>
#include <memory>
#include <unicode/translit.h>
#include <unicode/ucnv.h>     /* C   Converter API    */
#include <unicode/ustring.h>  /* some more string fcns*/
#include <unicode/utypes.h>   /* Basic ICU data types */


int main() {
  UErrorCode status = U_ZERO_ERROR;

  static char const * const samplestring = "If this compiles and runs without "
                                           "errors, apparently ICU is working.";

  auto const uLength = std::strlen(samplestring);
  auto const conv = ucnv_open("utf-8", &status);
  auto const uBufSize = uLength / ucnv_getMinCharSize(conv);
  auto const uBuf(std::make_unique<UChar[]>(uBufSize));

  ucnv_toUChars(conv, uBuf.get(), uLength,
        samplestring, uLength, &status);

  std::cout << samplestring << std::endl;
  return 0;
}
