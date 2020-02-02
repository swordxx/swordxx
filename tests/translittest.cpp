/******************************************************************************
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <string>
#include <swordxx/filters/utf8transliterator.h>
#include <vector>
#include <unicode/ucnv.h>     /* C   Converter API    */
#include <unicode/udata.h>    /* Data structures */
#include <unicode/ures.h>     /* Data structures */
#include <unicode/ustring.h>  /* some more string fcns*/
#include <unicode/utypes.h>   /* Basic ICU data types */
#include <unicode/translit.h>


using namespace std;

// Print the given string to stdout
void uprintf(const icu::UnicodeString &str) {
    int32_t len = str.length();
    // int32_t bufLen = str.extract(0, len, buf); // Preflight
    /* Preflighting seems to be broken now, so assume 1-1 conversion,
       plus some slop. */
    int32_t bufLen = len + 16;
        int32_t actualLen;
    std::vector<char> buf(bufLen + 1);
    actualLen = str.extract(0, len, buf.data()/*, bufLen*/); // Default codepage conversion
    buf[actualLen] = 0;
    //printf("%s", buf);
    std::cout << buf.data();
}


int main() {


  UErrorCode status = U_ZERO_ERROR;
//  UDataMemory *pappData = udata_open("/usr/local/lib/swordxx/swicu", "res", "root", &status);
  if (U_FAILURE(status))
  {
      std::cout << "error: " << status << ":" <<
        u_errorName(status) << std::endl;
    return 0;
  }
  UChar * uBuf;
  UChar * target;
  UConverter *conv;
  //UParseError perr = U_ZERO_ERROR;
  int32_t uBufSize = 0, uLength = 0;
//  void * pAppData = nullptr;
  const char * samplestring = "If this compiles and runs without errors, apparently ICU is working.";
  //ures_open("/usr/local/lib/swordxx/swicu.dat",
 //     nullptr, &status);
  //UDataMemory *pappData = udata_open("/usr/local/lib/swordxx/swicu",
//    "res", "root", &status);
  if (U_FAILURE(status))
  {
      std::cout << "error: " << status << ":" <<
        u_errorName(status) << std::endl;
    return 0;
  }
  //UDataMemory *pappData2 = udata_open("/usr/local/lib/swordxx/swicu",
//    "res", "translit_Latin_Gothic", &status);
  std::cout << status << std::endl;
  if (U_FAILURE(status))
  {
      std::cout << "error: " << status << ":" <<
        u_errorName(status) << std::endl;
    return 0;
  }
  std::cout << "available " << icu::Transliterator::countAvailableIDs() << std::endl;
  //udata_setAppData("/usr/local/lib/swordxx/swicu.dat" , pAppData, &status);
  //if (U_FAILURE(status))
  //{
      //std::cout << "error: " << status << ":" <<
    //    u_errorName(status) << std::endl;
    //return 0;
  //}

  int32_t i_ids = icu::Transliterator::countAvailableIDs();

  std::cout << "available " << i_ids << std::endl;
  for (int i=0; i<i_ids;i++)
  {
    std::cout << "id " << i << ": ";
    uprintf(icu::Transliterator::getAvailableID(i));
    std::cout << std::endl;
  }


  //UTF8Transliterator utran = new UTF8Transliterator();
  std::cout << "creating transliterator 2" << std::endl;
  std::unique_ptr<icu::Transliterator> btrans(icu::Transliterator::createInstance("NFD;Latin-Greek;NFC",
    UTRANS_FORWARD, status));
  if (U_FAILURE(status))
  {
      btrans.release();
      std::cout << "error: " << status << ":" <<
        u_errorName(status) << std::endl;
    return 0;
  }
  std::cout << "creating transliterator 1" << std::endl;
  std::unique_ptr<icu::Transliterator> trans(icu::Transliterator::createInstance("NFD;Latin-Gothic;NFC",
    UTRANS_FORWARD, status));
  if (U_FAILURE(status))
  {
      trans.release();
      std::cout << "error: " << status << ":" <<
        u_errorName(status) << std::endl;
    return 0;
  }

  std::cout << "deleting transliterator 1" << std::endl;
  trans.reset();
  std::cout << "deleting transliterator 2" << std::endl;
  btrans.reset();
  std::cout << "the rest" << std::endl;
  uLength = std::strlen(samplestring);
  conv = ucnv_open("utf-8", &status);
  if (U_FAILURE(status))
  {
      std::cout << "error: " << status << ":" <<
        u_errorName(status) << std::endl;
    return 0;
  }
  uBufSize = (uLength/ucnv_getMinCharSize(conv));
  uBuf = (UChar*)malloc(uBufSize * sizeof(UChar));

  target = uBuf;

  ucnv_toUChars(conv, target, uLength,
        samplestring, uLength, &status);
  if (U_FAILURE(status))
  {
      std::cout << "error: " << status << ":" <<
        u_errorName(status) << std::endl;
    return 0;
  }

  cout << samplestring << endl;

  return 0;
}
