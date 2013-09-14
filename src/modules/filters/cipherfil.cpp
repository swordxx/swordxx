/******************************************************************************
 *
 *  cipherfil.cpp -	CipherFilter, a SWFilter descendant to decipher
 *			a module
 *
 * $Id$
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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


#include <stdlib.h>
#include <cipherfil.h>
#include <swcipher.h>
#include <swbuf.h>


SWORD_NAMESPACE_START


CipherFilter::CipherFilter(const char *key) {
	cipher = new SWCipher((unsigned char *)key);
}


CipherFilter::~CipherFilter() {
	delete cipher;
}


SWCipher *CipherFilter::getCipher() {
	return cipher;
}


char CipherFilter::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	if (text.length() > 2) { //check if it's large enough to substract 2 in the next step.
		unsigned long len = text.length();
		if (!key) {	// hack, using key to determine encipher, or decipher
			cipher->cipherBuf(&len, text.getRawData()); //set buffer to enciphered text
			memcpy(text.getRawData(), cipher->Buf(), len);
//			text = cipher->Buf(); //get the deciphered buffer
		}
		else if ((unsigned long)key == 1) {
			cipher->Buf(text.getRawData(), len);
			memcpy(text.getRawData(), cipher->cipherBuf(&len), len);
//			text = cipher->cipherBuf(&len);
		}
	}
	return 0;
}


SWORD_NAMESPACE_END

