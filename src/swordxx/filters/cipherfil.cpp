/******************************************************************************
 *
 *  cipherfil.cpp -    CipherFilter, a SWFilter descendant to decipher
 *            a module
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

#include "cipherfil.h"

#include <cstdlib>
#include <cstring>
#include <string>


namespace swordxx {

char CipherFilter::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) module;
    if (text.length() > 2) { //check if it's large enough to substract 2 in the next step.
        std::size_t len = text.length();
        if (!key) {    // hack, using key to determine encipher, or decipher
            m_cipher.cipherBuf(&len, &text[0u]); //set buffer to enciphered text
            std::memcpy(&text[0u], m_cipher.buf(), len);
//            text = cipher->Buf(); //get the deciphered buffer
        }
        else if ((unsigned long)key == 1) {
            m_cipher.buf(&text[0u], len);
            std::memcpy(&text[0u], m_cipher.cipherBuf(&len), len);
//            text = cipher->cipherBuf(&len);
        }
    }
    return 0;
}


} /* namespace swordxx */

