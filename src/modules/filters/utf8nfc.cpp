/******************************************************************************
 *
 * utf8nfc - SWFilter descendant to perform NFC (canonical composition
 *                   normalization) on UTF-8 text
 *
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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

#include <stdlib.h>

#include <utilstr.h>
#include <unicode/unistr.h>
#include <unicode/normlzr.h>
#include <unicode/unorm.h>

#include <utf8nfc.h>
#include <swbuf.h>

SWORD_NAMESPACE_START

UTF8NFC::UTF8NFC() {
        conv = ucnv_open("UTF-8", &err);
}

UTF8NFC::~UTF8NFC() {
         ucnv_close(conv);
}

char UTF8NFC::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if ((unsigned long)key < 2)	// hack, we're en(1)/de(0)ciphering
		return -1;
        
	UErrorCode status = U_ZERO_ERROR;
	UnicodeString source(text.getRawData(), text.length(), conv, status);
	UnicodeString target;

	status = U_ZERO_ERROR;
	Normalizer::normalize(source, UNORM_NFC, 0, target, status);

	status = U_ZERO_ERROR;
	text.setSize(text.size()*2); // potentially, it can grow to 2x the original size
	int32_t len = target.extract(text.getRawData(), text.size(), conv, status);
	text.setSize(len);

	return 0;
}

SWORD_NAMESPACE_END
#endif
