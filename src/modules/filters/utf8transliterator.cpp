/******************************************************************************
*
* utf8transliterators - SWFilter decendant to transliterate between
*			 ICU-supported scripts.
*/

#ifdef _ICU_

#include <stdlib.h>
#include <string.h>

#ifdef __GNUC__
#include <unixstr.h>
#endif

#include <utf8transliterator.h>

const char UTF8Transliterator::optionstring[][NUMTARGETSCRIPTS] = {
       	"Off",
		"Latin", 
		"Greek",
		"Hebrew",
		"Cyrillic",
		"Arabic",
        "Syriac",
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
		"Beta/CCAT",
		"BGreek"
};

const char UTF8Transliterator::optName[] = "Transliteration";
const char UTF8Transliterator::optTip[] = "Transliterates between scripts";

UTF8Transliterator::UTF8Transliterator() {
	option = 0;
	   unsigned long i;
	for (i = 0; i < NUMTARGETSCRIPTS; i++) {
		options.push_back(optionstring[i]);
	}
}

void UTF8Transliterator::setOptionValue(const char *ival)
{
	unsigned char i = option = NUMTARGETSCRIPTS;
	while (i && stricmp(ival, optionstring[i])) {
		i--;
		option = i;
	}
}

const char *UTF8Transliterator::getOptionValue()
{
	return (NUMTARGETSCRIPTS > option) ? optionstring[option] : 0;
}

char UTF8Transliterator::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	if (option) {	// if we want transliteration
                UErrorCode err;
                bool noNFC = false;
                bool compat = false;

		// Convert UTF-8 string to UTF-16 (UChars)
                int32_t len;
                u_strFromUTF8(NULL, NULL, &len, text, -1, &err);
                len++;
                UChar *source = new UChar[len];
                u_strFromUTF8(source, len, &len, text, -1, &err);

		// Figure out which scripts are used in the string
		unsigned long i, j;
		unsigned char scripts[NUMSCRIPTS];

                for (i = 0; i < len; i++) {
                        j = ublock_getCode(source[i]);
			switch (j) {
			case U_BASIC_LATIN: scripts[SE_LATIN] = true; break;
			case U_GREEK: scripts[SE_GREEK] = true; break;
			case U_HEBREW: scripts[SE_HEBREW] = true; break;
			case U_CYRILLIC: scripts[SE_CYRILLIC] = true; break;
			case U_ARABIC: scripts[SE_ARABIC] = true; break;
			case U_SYRIAC: scripts[SE_SYRIAC] = true; break;                        
			case U_KATAKANA: scripts[SE_KATAKANA] = true; break;
			case U_HIRAGANA: scripts[SE_HIRAGANA] = true; break;
			case U_HANGUL_SYLLABLES: scripts[SE_HANGUL] = true;
			case U_HANGUL_JAMO: scripts[SE_JAMO] = true; break;
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
			case U_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A:
			case U_CJK_UNIFIED_IDEOGRAPHS:
                                if (lang[0] == 'z' && lang[1] == 'h')
                                        scripts[SE_HAN] = true;
                                else if (lang[0] == 'j' && lang[1] == 'a')
        				scripts[SE_KANJI] = true;
        			break;
			case U_CJK_COMPATIBILITY:
			case U_CJK_COMPATIBILITY_IDEOGRAPHS:
			case U_CJK_COMPATIBILITY_FORMS:
                                if (lang[0] == 'z' && lang[1] == 'h')
                                        scripts[SE_HAN] = true;
                                else if (lang[0] == 'j' && lang[1] == 'a')
        				scripts[SE_KANJI] = true;
                                compat = true;
        			break;
			case U_HANGUL_COMPATIBILITY_JAMO:
                                scripts[SE_JAMO] = true;
                                compat = true;
                                break;

                        default: scripts[SE_LATIN] = true;
			}
		} 
		scripts[option] = false; //turn off the reflexive transliteration
		
		//return if we have no transliteration to do for this text
		for (i = 0; !j && i < NUMSCRIPTS; i++) {
			if (scripts[i]) j++;
		}
		if (!j) return 0;

                UnicodeString id;
                if (compat) {
                        id = UnicodeString("NFKD");
                }
                else {
                        id = UnicodeString("NFD");
                }

		//Simple X to Latin transliterators
		if (scripts[SE_GREEK]) {
			if (option == SE_CCAT)
				id += UnicodeString(";Greek-Beta");
			else if (option == SE_BGREEK)
				id += UnicodeString(";Greek-BGreek");
			else {
				id += UnicodeString(";Greek-Latin");
				scripts[SE_LATIN] = true;
			}
		}
		if (scripts[SE_HEBREW]) {
			if (option == SE_CCAT)
				id += UnicodeString(";Hebrew-CCAT");
                        else if (option == SE_SYRIAC)
                                id += UnicodeString(";Hebrew-Syriac");
			else {
				id += UnicodeString(";Hebrew-Latin");
				scripts[SE_LATIN] = true;
			}
		}
		if (scripts[SE_CYRILLIC]) {
			id += UnicodeString(";Cyrillic-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_ARABIC]) {
			id += UnicodeString(";Arabic-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_SYRIAC]) {
                        if (option == SE_CCAT)
        			id += UnicodeString(";Syriac-CCAT");
                        else if (option == SE_HEBREW)
                                id += UnicodeString(";Syriac-Hebrew");
                        else {
        			id += UnicodeString(";Syriac-Latin");
        			scripts[SE_LATIN] = true;
                        }
		}
                if (scripts[SE_HAN]) {
			id += UnicodeString(";Han-Pinyin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_KANJI]) {
			id += UnicodeString(";Kanji-OnRomaji");
			scripts[SE_LATIN] = true;
		}

       		// Inter-Kana and Kana to Latin transliterators
		if (option == SE_HIRAGANA && scripts[SE_KATAKANA]) {
			id += UnicodeString(";Katakana-Hiragana");
			scripts[SE_HIRAGANA] = true;
		}
		else if (option == SE_KATAKANA && scripts[SE_HIRAGANA]) {
			id += UnicodeString(";Hiragana-Katakana");
			scripts[SE_KATAKANA] = true;
		}
		else {
        		if (scripts[SE_KATAKANA]) {
	        		id += UnicodeString(";Katakana-Latin");
		        	scripts[SE_LATIN] = true;
        		}
	        	if (scripts[SE_HIRAGANA]) {
		        	id += UnicodeString(";Hiragana-Latin");
			        scripts[SE_LATIN] = true;
        		}
                }

		// Inter-Korean and Korean to Latin transliterators
		if (option == SE_HANGUL && scripts[SE_JAMO]) {
			id += UnicodeString(";Jamo-Hangul");
			scripts[SE_HANGUL] = true;
		}
		else if (option == SE_JAMO && scripts[SE_HANGUL]) {
			id += UnicodeString(";Hangul-Jamo");
			scripts[SE_JAMO] = true;
		}
		else {
			if (scripts[SE_HANGUL]) {
				id += UnicodeString(";Hangul-Jamo");
				scripts[SE_JAMO] = true;
			}
			if (scripts[SE_JAMO]) {
				id += UnicodeString(";Jamo-Latin");
				scripts[SE_LATIN] = true;
			}
		}

		// Indic-Latin
		if (option < SE_DEVANAGARI || option > SE_MALAYALAM) {
			// Indic to Latin
			if (scripts[SE_TAMIL]) {
				id += UnicodeString(";Tamil-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_BENGALI]) {
				id += UnicodeString(";Bengali-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_GURMUKHI]) {
				id += UnicodeString(";Gurmukhi-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_GUJARATI]) {
				id += UnicodeString(";Gujarati-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_ORIYA]) {
				id += UnicodeString(";Oriya-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_TELUGU]) {
				id += UnicodeString(";Telugu-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_KANNADA]) {
				id += UnicodeString(";Kannada-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_MALAYALAM]) {
				id += UnicodeString(";Malayalam-Latin");
				scripts[SE_LATIN] = true;
			}
		}
		else {
			if (scripts[SE_LATIN]) {
				id += UnicodeString(";Latin-InterIndic");
			}
			if (scripts[SE_DEVANAGARI]) {
				id += UnicodeString(";Devanagari-InterIndic");
			}
			if (scripts[SE_TAMIL]) {
				id += UnicodeString(";Tamil-InterIndic");
			}
			if (scripts[SE_BENGALI]) {
				id += UnicodeString(";Bengali-InterIndic");
			}
			if (scripts[SE_GURMUKHI]) {
				id += UnicodeString(";Gurmurkhi-InterIndic");
			}
			if (scripts[SE_GUJARATI]) {
				id += UnicodeString(";Gujarati-InterIndic");
			}
			if (scripts[SE_ORIYA]) {
				id += UnicodeString(";Oriya-InterIndic");
			}
			if (scripts[SE_TELUGU]) {
				id += UnicodeString(";Telugu-InterIndic");
			}
			if (scripts[SE_KANNADA]) {
				id += UnicodeString(";Kannada-InterIndic");
			}
			if (scripts[SE_MALAYALAM]) {
				id += UnicodeString(";Malayalam-InterIndic");
			}

			switch(option) {
			case SE_DEVANAGARI:
				id += UnicodeString(";InterIndic-Devanagari");
				break;
			case SE_TAMIL:
				id += UnicodeString(";InterIndic-Tamil");
				break;
			case SE_BENGALI:
				id += UnicodeString(";InterIndic-Bengali");
				break;
			case SE_GURMUKHI:
				id += UnicodeString(";InterIndic-Gurmukhi");
				break;
			case SE_GUJARATI:
				id += UnicodeString(";InterIndic-Gujarati");
				break;
			case SE_ORIYA:
				id += UnicodeString(";InterIndic-Oriya");
				break;
			case SE_TELUGU:
				id += UnicodeString(";InterIndic-Telugu");
				break;
			case SE_KANNADA:
				id += UnicodeString(";InterIndic-Kannada");
				break;
			case SE_MALAYALAM:
				id += UnicodeString(";InterIndic-Malayalam");
				break;
			default:
				id += UnicodeString(";InterIndic-Latin");
				scripts[SE_LATIN] = true;
				break;
			}
		}

		if (scripts[SE_LATIN]) {
                switch (option) {
                        case SE_GREEK:
				id += UnicodeString(";Latin-Greek");
                                break;
                        case SE_HEBREW:
				id += UnicodeString(";Latin-Hebrew");
                                break;
                        case SE_CYRILLIC:
				id += UnicodeString(";Latin-Cyrillic");
                                break;
                        case SE_ARABIC:
				id += UnicodeString(";Latin-Arabic");
                                break;
                        case SE_SYRIAC:
				id += UnicodeString(";Latin-Syriac");
                                break;
                        case SE_KATAKANA:
				id += UnicodeString(";Latin-Katakana");
                                break;
                        case SE_HIRAGANA:
				id += UnicodeString(";Latin-Hiragana");
                                break;
                        case SE_JAMO:
				id += UnicodeString(";Latin-Jamo");
                                noNFC = true;
                                break;
                        case SE_HANGUL:
				id += UnicodeString(";Latin-Hangul");
                                break;
                }
		}

        if (!noNFC) {
				id += UnicodeString(";NFC");
		}

        UParseError perr;
        Transliterator * trans = Transliterator::createFromRules("", id.getBuffer(), UTRANS_FORWARD, perr, err);
        UnicodeString target = UnicodeString(source);
        trans->transliterate(target);
        u_strToUTF8(text, maxlen, NULL, target.getBuffer(), target.length(), &err);
        *(text + maxlen) = 0;
        delete trans;
	}
	return 0;
}
#endif
