/******************************************************************************
*
* utf8arshaping - SWFilter decendant to perform Arabic shaping on
*                   UTF-8 text
*/

#ifdef _ICU_

#include <stdlib.h>
#include <string.h>

#ifdef __GNUC__
#include <unixstr.h>
#endif

#include <utf8arshaping.h>

UTF8arShaping::UTF8arShaping() {

        conv = ucnv_open("UTF-8", &err);

}

UTF8arShaping::~UTF8arShaping() {
        ucnv_close(conv);
}

char UTF8arShaping::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
        UChar *ustr, *ustr2;
	 if ((unsigned long)key < 2)	// hack, we're en(1)/de(0)ciphering
		return -1;

        int32_t len = strlen(text);
        ustr = new UChar[len];
        ustr2 = new UChar[len];

	// Convert UTF-8 string to UTF-16 (UChars)
        len = ucnv_toUChars(conv, ustr, len, text, -1, &err);

        len = u_shapeArabic(ustr, len, ustr2, len, U_SHAPE_LETTERS_SHAPE | U_SHAPE_DIGITS_EN2AN, &err);

        ucnv_fromUChars(conv, text, maxlen, ustr2, len, &err);

        delete [] ustr2;
        delete [] ustr;
	return 0;
}

#endif
