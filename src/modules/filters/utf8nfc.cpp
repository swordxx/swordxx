/******************************************************************************
*
* utf8nfc - SWFilter decendant to perform NFC (canonical composition
*                   normalization) on UTF-8 text
*/

#ifdef _ICU_

#include <stdlib.h>
#include <string.h>

#ifdef __GNUC__
#include <unixstr.h>
#endif

#include <utf8nfc.h>

UTF8NFC::UTF8NFC() {
        conv = ucnv_open("UTF-8", &err);
}

UTF8NFC::~UTF8NFC() {
         ucnv_close(conv);
}

char UTF8NFC::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
        int32_t len = strlen(text) * 2;
        source = new UChar[len + 1]; //each char could become a surrogate pair

	// Convert UTF-8 string to UTF-16 (UChars)
        len = ucnv_toUChars(conv, source, len, text, -1, &err);
        target = new UChar[len + 1];

        //canonical composition
        unorm_normalize(source, len, UNORM_NFC, 0, target, len, &err);

        ucnv_fromUChars(conv, text, maxlen, target, -1, &err);

        delete [] source;
        delete [] target;

	return 0;
}

#endif
