/******************************************************************************
*
* utf8cnormalizer - SWFilter decendant to perform reordering of UTF-8
*                   text to visual order according to Unicode BiDi
*/

#ifdef _ICU_

#include <stdlib.h>
#include <string.h>

#ifdef __GNUC__
#include <unixstr.h>
#endif

#include <utf8bidireorder.h>

UTF8BiDiReorder::UTF8BiDiReorder() {

        conv = ucnv_open("UTF-8", &err);

}

UTF8BiDiReorder::~UTF8BiDiReorder() {
        ucnv_close(conv);
}

char UTF8BiDiReorder::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
        UChar *ustr, *ustr2;
        
        int32_t len = strlen(text);
        ustr = new UChar[len]; //each char could become a surrogate pair

	// Convert UTF-8 string to UTF-16 (UChars)
        len = ucnv_toUChars(conv, ustr, len, text, -1, &err);
        ustr2 = new UChar[len];

        UBiDi* bidi = ubidi_openSized(len + 1, 0, &err);
        ubidi_setPara(bidi, ustr, len, UBIDI_DEFAULT_RTL, NULL, &err);
        len = ubidi_writeReordered(bidi, ustr2, len,
                UBIDI_DO_MIRRORING | UBIDI_REMOVE_BIDI_CONTROLS, &err);
        ubidi_close(bidi);

//        len = ubidi_writeReverse(ustr, len, ustr2, len,
//                UBIDI_DO_MIRRORING | UBIDI_REMOVE_BIDI_CONTROLS, &err);

        ucnv_fromUChars(conv, text, maxlen, ustr2, len, &err);

        delete [] ustr2;
        delete [] ustr;
	return 0;
}

#endif
