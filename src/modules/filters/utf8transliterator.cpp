/******************************************************************************
*
* utf8transliterators - SWFilter decendant to transliterate between
*			 ICU-supported scripts.
*/

#ifdef ICU

#include <stdlib.h>
#include <string.h>

#ifdef __GNUC__
#include <unixstr.h>
#endif

#include <utf8transliterator.h>

#include <unicode/utypes.h>
#include <unicode/translit.h>
#include <unicode/convert.h>
#include <unicode/ustring.h>

const char UTF8Transliterator::optionstring[][NUMSCRIPTS] = {
	"Off", 
		"Latin", 
		"Greek",
		"Hebrew",
		"Cyrillic",
		"Arabic",
		"Kana",
		"Jamo",
		"Hangul",
		"Devanagari",
		"Tamil",
		"Bengali",
		"Gurmukhi",
		"Gujarati",
		"Oriya",
		"Telugu",
		"Kannada",
		"Malayalam",
		"Maltese",
		"BETA",
		"BGreek",
		"BHebrew"
};

const char UTF8Transliterator::optName[] = "Transliteration";
const char UTF8Transliterator::optTip[] = "Transliterates between scripts";


UTF8Transliterator::UTF8Transliterator() {
	option = 0;
	for (unsigned char i = 0; i <= NUMSCRIPTS; i++) {
		options.push_back(optionstring[i]);
	}
}

UTF8Transliterator::~UTF8Transliterator() {
}

void UTF8Transliterator::setOptionValue(const char *ival)
{
	unsigned char i = option = NUMSCRIPTS;
	while (i && stricmp(ival, optionstring[i])) {
		i--;
		option = i;
	}
}

const char *UTF8Transliterator::getOptionValue()
{
	return (NUMSCRIPTS >= option) ? optionstring[option] : 0;
}

char UTF8Transliterator::ProcessText(char *text, int maxlen, const SWKey *key)
{
	if (option) {	// if we want transliteration
		long len;
		
		len = strlen(text) + 1;
		// -------------------------------
		
		// Convert UTF-8 string to UTF-16 (UChars)
		int32_t ulen;
		UErrorCode status = U_ZERO_ERROR;
		static UnicodeConverter conv("utf-8", status);
		static UnicodeString ustr;
		conv.toUnicodeString(ustr, text, len, status);
		ulen = ustr.length();
		
		// Figure out which scripts are used in the string
		unsigned long i, j;
		unsigned char scripts[NUMSCRIPTS];
		for (i = 0; i < NUMSCRIPTS; i++) {
			scripts[i] = false;
		}
		for (i = 0; i < ulen; i++) {
			j = u_charScript(ustr.char32At(i));
			switch (j) {
			case U_BASIC_LATIN: scripts[SE_LATIN] = true; break;
			case U_GREEK: scripts[SE_GREEK] = true; break;
			case U_HEBREW: scripts[SE_HEBREW] = true; break;
			case U_CYRILLIC: scripts[SE_CYRILLIC] = true; break;
			case U_ARABIC: scripts[SE_ARABIC] = true; break;
			case U_KATAKANA:
			case U_HIRAGANA: scripts[SE_KANA] = true; break;
			case U_HANGUL_SYLLABLES: scripts[SE_HANGUL] = true;
			case U_HANGUL_JAMO:
			case U_HANGUL_COMPATIBILITY_JAMO: scripts[SE_JAMO] = true; break;
			case U_DEVANAGARI: scripts[SE_DEVANAGARI] = true; break;
			case U_TAMIL: scripts[SE_TAMIL] = true; break;
			case U_BENGALI: scripts[SE_BENGALI] = true; break;
			case U_GURMUKHI: scripts[SE_GURMUKHI] = true; break;
			case U_GUJARATI: scripts[SE_GUJARATI] = true; break;
			case U_ORIYA: scripts[SE_ORIYA] = true; break;
			case U_TELUGU: scripts[SE_TELUGU] = true; break;
			case U_KANNADA: scripts[SE_KANNADA] = true; break;
			case U_MALAYALAM: scripts[SE_MALAYALAM] = true; break;
			case U_CJK_RADICALS_SUPPLEMENT:
			case U_KANGXI_RADICALS:
			case U_IDEOGRAPHIC_DESCRIPTION_CHARACTERS:
			case U_CJK_SYMBOLS_AND_PUNCTUATION:
			case U_CJK_COMPATIBILITY:
			case U_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A:
			case U_CJK_UNIFIED_IDEOGRAPHS:
			case U_CJK_COMPATIBILITY_IDEOGRAPHS:
			case U_CJK_COMPATIBILITY_FORMS: scripts[SE_HAN] = true;
				scripts[SE_KANJI] = true;
				break;
				//default: scripts[SE_LATIN] = true;
			}
		} 
		scripts[option] = false; //turn off the reflexive transliteration
		
		//return if we have no transliteration to do for this text
		for (i = 0; !j && i < NUMSCRIPTS; i++) {
			if (scripts[i]) j++;
		}
		if (!j) return 0;
		
		Transliterator* trans;
		UParseError perror;
		static UnicodeString id;

		//Simple X to Latin transliterators
		if (scripts[SE_GREEK]) {
			if (option == SE_BETA)
				id = UnicodeString("Greek-BETA", "");
			else if (option == SE_BGREEK)
				id = UnicodeString("Greek-BGreek", "");
			else {
				id = UnicodeString("Greek-Latin", "");
				scripts[SE_LATIN] = true;
			}
			trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
			trans->transliterate(ustr);
			delete trans;
		}
		if (scripts[SE_HEBREW]) {
			if (option == SE_BHEBREW)
				id = UnicodeString("Hebrew-BHebrew", "");
			else {
				id = UnicodeString("Hebrew-Latin", "");
				scripts[SE_LATIN] = true;
			}
			trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
			trans->transliterate(ustr);
			delete trans;
		}
		if (scripts[SE_CYRILLIC]) {
			id = UnicodeString("Cyrillic-Latin", "");
			scripts[SE_LATIN] = true;
			trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
			trans->transliterate(ustr);
			delete trans;
		}
		if (scripts[SE_ARABIC]) {
			id = UnicodeString("Arabic-Latin", "");
			scripts[SE_LATIN] = true;
			trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
			trans->transliterate(ustr);
			delete trans;
		}
		if (scripts[SE_KANA]) {
			id = UnicodeString("Kana-Latin", "");
			scripts[SE_LATIN] = true;
			trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
			trans->transliterate(ustr);
			delete trans;
		}
		if (scripts[SE_HAN]) {
			id = UnicodeString("Han-Pinyin", "");
			scripts[SE_LATIN] = true;
			trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
			trans->transliterate(ustr);
			delete trans;
		}
		if (scripts[SE_KANJI]) {
			id = UnicodeString("Kanji-OnRomaji", "");
			scripts[SE_LATIN] = true;
			trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
			trans->transliterate(ustr);
			delete trans;
		}
		
		// Inter-Korean and Korean to Latin transliterators
		if (option == SE_HANGUL && scripts[SE_JAMO]) {
			id = UnicodeString("Jamo-Hangul", "");
			scripts[SE_HANGUL] = true;
			trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
			trans->transliterate(ustr);
			delete trans;
		}
		else if (option == SE_JAMO && scripts[SE_HANGUL]) {
			id = UnicodeString("Hangul-Jamo", "");
			scripts[SE_JAMO] = true;
			trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
			trans->transliterate(ustr);
			delete trans;
		}
		else {
			if (scripts[SE_HANGUL]) {
				id = UnicodeString("Hangul-Jamo", "");
				scripts[SE_JAMO] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_JAMO]) {
				id = UnicodeString("Jamo-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
		}
		
		// Indic-Latin
		if (option < SE_DEVANAGARI || option > SE_MALAYALAM) {
			// Indic to Latin
			if (scripts[SE_TAMIL]) {
				id = UnicodeString("Tamil-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_BENGALI]) {
				id = UnicodeString("Bengali-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_GURMUKHI]) {
				id = UnicodeString("Gurmukhi-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_GUJARATI]) {
				id = UnicodeString("Gujarati-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_ORIYA]) {
				id = UnicodeString("Oriya-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_TELUGU]) {
				id = UnicodeString("Telugu-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_KANNADA]) {
				id = UnicodeString("Kannada-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_MALAYALAM]) {
				id = UnicodeString("Malayalam-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
		}
		else {
			if (scripts[SE_LATIN]) {
				id = UnicodeString("Latin-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_DEVANAGARI]) {
				id = UnicodeString("Devanagari-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_TAMIL]) {
				id = UnicodeString("Tamil-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_BENGALI]) {
				id = UnicodeString("Bengali-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_GURMUKHI]) {
				id = UnicodeString("Gurmurkhi-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_GUJARATI]) {
				id = UnicodeString("Gujarati-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_ORIYA]) {
				id = UnicodeString("Oriya-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_TELUGU]) {
				id = UnicodeString("Telugu-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_KANNADA]) {
				id = UnicodeString("Kannada-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			if (scripts[SE_MALAYALAM]) {
				id = UnicodeString("Malayalam-InterIndic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}

			
			switch(option) {
			case SE_DEVANAGARI:
				id = UnicodeString("InterIndic-Devanagari", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			case SE_TAMIL:
				id = UnicodeString("InterIndic-Tamil", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			case SE_BENGALI:
				id = UnicodeString("InterIndic-Bengali", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			case SE_GURMUKHI:
				id = UnicodeString("InterIndic-Gurmukhi", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			case SE_GUJARATI:
				id = UnicodeString("InterIndic-Gujarati", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			case SE_ORIYA:
				id = UnicodeString("InterIndic-Oriya", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			case SE_TELUGU:
				id = UnicodeString("InterIndic-Telugu", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			case SE_KANNADA:
				id = UnicodeString("InterIndic-Kannada", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			case SE_MALAYALAM:
				id = UnicodeString("InterIndic-Malayalam", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			default:
				id = UnicodeString("InterIndic-Latin", "");
				scripts[SE_LATIN] = true;
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				break;
			}
		}
		
		if (scripts[SE_LATIN]) {
			if (option == SE_GREEK) {
				id = UnicodeString("Latin-Greek", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			else if (option == SE_HEBREW) {
				id = UnicodeString("Latin-Hebrew", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			else if (option == SE_CYRILLIC) {
				id = UnicodeString("Latin-Cyrillic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			else if (option == SE_ARABIC) {
				id = UnicodeString("Latin-Arabic", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			else if (option == SE_KANA) {
				id = UnicodeString("Latin-Kana", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			else if (option == SE_JAMO) {
				id = UnicodeString("Latin-Jamo", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			else if (option == SE_HANGUL) {
				id = UnicodeString("Latin-Jamo", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
				id = UnicodeString("Jamo-Hangul", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
			else if (option == SE_MALTESE) {
				if = UnicodeString("Latin-Maltese", "");
				trans = Transliterator::createInstance(id, UTRANS_FORWARD, &perror);
				trans->transliterate(ustr);
				delete trans;
			}
		}
		//		int32_t newlen = ((unsigned int)maxlen) > sizeof(text) ? maxlen : sizeof(text);
		conv.fromUnicodeString(text, maxlen, ustr, status);
	}
	return 0;
}
#endif