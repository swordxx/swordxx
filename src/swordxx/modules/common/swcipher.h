/******************************************************************************
 *
 *  swcipher.h -    definition of Class SWCipher used for data
 *            cipher/decipher
 *
 * $Id$
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_SWCIPHER_H
#define SWORDXX_SWCIPHER_H

#include "sapphire.h"

#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT SWCipher
{

  Sapphire master;
  Sapphire work;

  char *buf;
  bool cipher;
  unsigned long len;
protected:
public:
    SWCipher(char const * key);
  virtual void setCipherKey (const char *key);
    virtual ~ SWCipher ();
  virtual char * Buf(char const * buf = nullptr, unsigned long len = 0u);
  virtual char * cipherBuf(unsigned long * len, char const * buf = nullptr);
  virtual void Encode (void);
  virtual void Decode (void);
};

} /* namespace swordxx */

#endif /* SWORDXX_SWCIPHER_H */
