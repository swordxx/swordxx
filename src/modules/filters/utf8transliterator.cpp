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

const char UTF8Transliterator::optionstring[NUMTARGETSCRIPTS][16] = {
        "Off",
        "Latin",
        "Basic Latin",
        "Beta",
        "BGreek",
/*
        "Greek",
        "Hebrew",
        "Cyrillic",
        "Arabic",
        "Syriac",
        "Katakana",
        "Hiragana",
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
        "Thai",
        "Georgian",
        "Armenian",
        "Ethiopic",
        "Gothic",
        "Ugaritic",
        "Coptic"
        */
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
		unsigned long i, j;
                UErrorCode err = U_ZERO_ERROR;
                UConverter * conv = NULL;
                conv = ucnv_open("UTF-8", &err);

                bool compat = false;
                bool noNFC = false;

                if (option == SE_JAMO) {
                        noNFC = true;
                }

		// Convert UTF-8 string to UTF-16 (UChars)
                j = strlen(text);
                int32_t len = (j * 2) + 1;
                UChar *source = new UChar[len];
                err = U_ZERO_ERROR;
                len = ucnv_toUChars(conv, source, len, text, j, &err);
                source[len] = 0;

		// Figure out which scripts are used in the string
		unsigned char scripts[NUMSCRIPTS];

                for (i = 0; i < NUMSCRIPTS; i++) {
                        scripts[i] = false;
                }

                for (i = 0; i < len; i++) {
                        j = ublock_getCode(source[i]);
			switch (j) {
			case UBLOCK_BASIC_LATIN: scripts[SE_LATIN] = true; break;
			case UBLOCK_GREEK: scripts[SE_GREEK] = true; break;
			case UBLOCK_HEBREW: scripts[SE_HEBREW] = true; break;
			case UBLOCK_CYRILLIC: scripts[SE_CYRILLIC] = true; break;
			case UBLOCK_ARABIC: scripts[SE_ARABIC] = true; break;
			case UBLOCK_SYRIAC: scripts[SE_SYRIAC] = true; break;
			case UBLOCK_KATAKANA: scripts[SE_KATAKANA] = true; break;
			case UBLOCK_HIRAGANA: scripts[SE_HIRAGANA] = true; break;
			case UBLOCK_HANGUL_SYLLABLES: scripts[SE_HANGUL] = true; break;
			case UBLOCK_HANGUL_JAMO: scripts[SE_JAMO] = true; break;
			case UBLOCK_DEVANAGARI: scripts[SE_DEVANAGARI] = true; break;
			case UBLOCK_TAMIL: scripts[SE_TAMIL] = true; break;
			case UBLOCK_BENGALI: scripts[SE_BENGALI] = true; break;
			case UBLOCK_GURMUKHI: scripts[SE_GURMUKHI] = true; break;
			case UBLOCK_GUJARATI: scripts[SE_GUJARATI] = true; break;
			case UBLOCK_ORIYA: scripts[SE_ORIYA] = true; break;
			case UBLOCK_TELUGU: scripts[SE_TELUGU] = true; break;
			case UBLOCK_KANNADA: scripts[SE_KANNADA] = true; break;
			case UBLOCK_MALAYALAM: scripts[SE_MALAYALAM] = true; break;
			case UBLOCK_THAI: scripts[SE_THAI] = true; break;
			case UBLOCK_GEORGIAN: scripts[SE_GEORGIAN] = true; break;
      			case UBLOCK_ARMENIAN: scripts[SE_ARMENIAN] = true; break;
                        case UBLOCK_ETHIOPIC: scripts[SE_ETHIOPIC] = true; break;
                        case UBLOCK_GOTHIC: scripts[SE_GOTHIC] = true; break;
                        // needs Unicode 3.2? or 4.0? support from ICU
                        //case UBLOCK_UGARITIC: scripts[SE_UGARITIC] = true; break;
			case UBLOCK_CJK_RADICALS_SUPPLEMENT:
			case UBLOCK_KANGXI_RADICALS:
			case UBLOCK_IDEOGRAPHIC_DESCRIPTION_CHARACTERS:
			case UBLOCK_CJK_SYMBOLS_AND_PUNCTUATION:
			case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A:
			case UBLOCK_CJK_UNIFIED_IDEOGRAPHS:
                                scripts[SE_HAN] = true;
                                break;
			case UBLOCK_CJK_COMPATIBILITY:
			case UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS:
			case UBLOCK_CJK_COMPATIBILITY_FORMS:
                                scripts[SE_HAN] = true;
                                compat = true;
        			break;
			case UBLOCK_HANGUL_COMPATIBILITY_JAMO:
                                scripts[SE_HANGUL] = true;
                                compat = true;
                                break;

                        default: scripts[SE_LATIN] = true;
			}
		} 
		scripts[option] = false; //turn off the reflexive transliteration
		
		//return if we have no transliteration to do for this text
                j = 0;
               	for (i = 0; !j && i < NUMSCRIPTS; i++) {
	        	if (scripts[i]) j++;
        	}
	       	if (!j) {
                        ucnv_close(conv);
                        return 0;
                }

                UnicodeString id;
                if (compat) {
                        id = UnicodeString("NFKD");
                }
                else {
                        id = UnicodeString("NFD");
                }

		//Simple X to Latin transliterators
		if (scripts[SE_GREEK]) {
			if (option == SE_BETA)
				id += UnicodeString(";Greek-Beta");
			else if (option == SE_BGREEK)
				id += UnicodeString(";Greek-BGreek");
			else {
                                if (!strnicmp (module->Lang(), "cop", 3)) {
        				id += UnicodeString(";Coptic-Latin");
                                }
                                else {
        				id += UnicodeString(";Greek-Latin");
                                }
				scripts[SE_LATIN] = true;
			}
		}
		if (scripts[SE_HEBREW]) {
			if (option == SE_BETA)
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
                        if (option == SE_BETA)
        			id += UnicodeString(";Syriac-CCAT");
                        else if (option == SE_HEBREW)
                                id += UnicodeString(";Syriac-Hebrew");
                        else {
        			id += UnicodeString(";Syriac-Latin");
        			scripts[SE_LATIN] = true;
                        }
		}
		if (scripts[SE_THAI]) {
			id += UnicodeString(";Thai-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_GEORGIAN]) {
			id += UnicodeString(";Georgian-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_ARMENIAN]) {
			id += UnicodeString(";Armenian-Latin");
			scripts[SE_LATIN] = true;
		}                
		if (scripts[SE_ETHIOPIC]) {
			id += UnicodeString(";Ethiopic-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_GOTHIC]) {
			id += UnicodeString(";Gothic-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_UGARITIC]) {
			id += UnicodeString(";Ugaritic-Latin");
			scripts[SE_LATIN] = true;
		}
                if (scripts[SE_HAN]) {
                        if (!strnicmp (module->Lang(), "ja", 2)) {
        			id += UnicodeString(";Kanji-OnRomaji");
                        }
                        else {
        			id += UnicodeString(";Han-Pinyin");
                        }
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
			noNFC = false;
			scripts[SE_HANGUL] = true;
		}
		else if (option == SE_JAMO && scripts[SE_HANGUL]) {
			noNFC = true;
			scripts[SE_JAMO] = true;
		}
		else {
			if (scripts[SE_HANGUL]) {
				id += UnicodeString(";Hangul-Latin");
				scripts[SE_LATIN] = true;
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
                        case SE_THAI:
				id += UnicodeString(";Latin-Thai");
                                break;
                        case SE_GEORGIAN:
				id += UnicodeString(";Latin-Georgian");
                                break;
                        case SE_ARMENIAN:
				id += UnicodeString(";Latin-Armenian");
                                break;
                        case SE_ETHIOPIC:
				id += UnicodeString(";Latin-Ethiopic");
                                break;
                        case SE_GOTHIC:
				id += UnicodeString(";Latin-Gothic");
                                break;
                        case SE_UGARITIC:
				id += UnicodeString(";Latin-Ugaritic");
                                break;
                        case SE_COPTIC:
				id += UnicodeString(";Latin-Coptic");
                                break;
                        case SE_KATAKANA:
				id += UnicodeString(";Latin-Katakana");
                                break;
                        case SE_HIRAGANA:
				id += UnicodeString(";Latin-Hiragana");
                                break;
                        case SE_JAMO:
				id += UnicodeString(";Latin-Jamo");
                                break;
                        case SE_HANGUL:
				id += UnicodeString(";Latin-Hangul");
                                break;
                        }
                }

                if (option == SE_BASICLATIN) {
                        id += UnicodeString(";Any-Latin1");
                }
                                
                if (noNFC) {
                        id += UnicodeString(";NFD");
                } else {
                        id += UnicodeString(";NFC");
                }

                UParseError perr;

                err = U_ZERO_ERROR;
                Transliterator * trans = Transliterator::createInstance(id, UTRANS_FORWARD, perr, err);
                if (trans) {
                        UnicodeString target = UnicodeString(source);
                        trans->transliterate(target);
                        len = ucnv_fromUChars(conv, text, maxlen, target.getBuffer(), target.length(), &err);
                        if (len < maxlen) *(text + len) = 0;
                        else *(text + maxlen) = 0;
                        delete trans;
                }
                ucnv_close(conv);
        }
	return 0;
}
#endif
