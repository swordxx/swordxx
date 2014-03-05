/******************************************************************************
 *
 *  utf8nfkd.cpp -	SWFilter descendant to perform NFKD (compatability
 *			decomposition normalization) on UTF-8 text
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

#ifdef _ICU_

#include <utf8nfkd.h>
#include <swbuf.h>


SWORD_NAMESPACE_START


UTF8NFKD::UTF8NFKD() {
        conv = ucnv_open("UTF-8", &err);
}


UTF8NFKD::~UTF8NFKD() {
         ucnv_close(conv);
}


char UTF8NFKD::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if ((unsigned long)key < 2)	// hack, we're en(1)/de(0)ciphering
		return -1;
        
	int32_t len =  5 + text.length() * 5;
        source = new UChar[len + 1]; //each char could become a surrogate pair

	// Convert UTF-8 string to UTF-16 (UChars)
        int32_t ulen = ucnv_toUChars(conv, source, len, text.c_str(), -1, &err);
        target = new UChar[len + 1];

        //compatability decomposition
        ulen = unorm_normalize(source, ulen, UNORM_NFKD, 0, target, len, &err);

	   text.setSize(len);
	   len = ucnv_fromUChars(conv, text.getRawData(), len, target, ulen, &err);
	   text.setSize(len);

	   delete [] source;
	   delete [] target;

	return 0;
}


SWORD_NAMESPACE_END
#endif
