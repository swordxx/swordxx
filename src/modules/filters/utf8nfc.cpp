/******************************************************************************
*
* utf8nfc - SWFilter descendant to perform NFC (canonical composition
*                   normalization) on UTF-8 text
*/

#ifdef _ICU_

#include <stdlib.h>

#include <utilstr.h>

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
        
	int32_t len = text.length() * 2;
        source = new UChar[len + 1]; //each char could become a surrogate pair

	// Convert UTF-8 string to UTF-16 (UChars)
        len = ucnv_toUChars(conv, source, len, text.c_str(), -1, &err);
        target = new UChar[len + 1];

        //canonical composition
        unorm_normalize(source, len, UNORM_NFC, 0, target, len, &err);

	   text.setSize(text.size()*2);
	   len = ucnv_fromUChars(conv, text.getRawData(), text.size(), target, -1, &err);
	   text.setSize(len);

        delete [] source;
        delete [] target;

	return 0;
}

SWORD_NAMESPACE_END
#endif
