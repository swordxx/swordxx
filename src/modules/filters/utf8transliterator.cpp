/******************************************************************************
*
* utf8transliterators - SWFilter descendant to transliterate between
*			 ICU-supported scripts.
*/

#ifdef _ICU_

#include <stdlib.h>

#ifdef __GNUC__
#include <unixstr.h>
#endif

#include <unicode/ucnv.h>
#include <unicode/uchar.h>
#include <utf8transliterator.h>

#ifndef _ICUSWORD_
#include "unicode/resbund.h"
#endif
#include <swlog.h>

SWORD_NAMESPACE_START

#if 0
#ifdef _ICU_
class UnicodeCaster {
	const UnicodeString &ustr;
public:
	UnicodeCaster(const UnicodeString &ustr):ustr(ustr) {}; operator const char *() { return ""; };
};

#endif
#endif
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

SWTransMap UTF8Transliterator::transMap;

#ifndef _ICUSWORD_

const char UTF8Transliterator::SW_RB_RULE_BASED_IDS[] = "RuleBasedTransliteratorIDs";
const char UTF8Transliterator::SW_RB_RULE[] = "Rule";
#ifdef SWICU_DATA
const char UTF8Transliterator::SW_RESDATA[] = SWICU_DATA;
#else
const char UTF8Transliterator::SW_RESDATA[] = "/usr/local/lib/sword/";
#endif

class SWCharString {
 public:
    inline SWCharString(const UnicodeString& str);
    inline ~SWCharString();
    inline operator const char*() { return ptr; }
 private:
    char buf[128];
    char* ptr;
};
SWCharString::SWCharString(const UnicodeString& str) {
    // TODO This isn't quite right -- we should probably do
    // preflighting here to determine the real length.
    if (str.length() >= (int32_t)sizeof(buf)) {
        ptr = new char[str.length() + 8];
    } else {
        ptr = buf;
    }
    str.extract(0, 0x7FFFFFFF, ptr, "");
}

SWCharString::~SWCharString() {
    if (ptr != buf) {
        delete[] ptr;
    }
}

#endif // _ICUSWORD_


UTF8Transliterator::UTF8Transliterator() {
	option = 0;
        unsigned long i;
	for (i = 0; i < NUMTARGETSCRIPTS; i++) {
		options.push_back(optionstring[i]);
	}
#ifndef _ICUSWORD_
	utf8status = U_ZERO_ERROR;
	Load(utf8status);
#endif
}

void UTF8Transliterator::Load(UErrorCode &status)
{
#ifndef _ICUSWORD_
	static const char translit_swordindex[] = "translit_swordindex";
	
    UResourceBundle *bundle, *transIDs, *colBund;
    bundle = ures_openDirect(SW_RESDATA, translit_swordindex, &status);
    if (U_FAILURE(status)) {
		SWLog::systemlog->LogError("no resource index to load");
		SWLog::systemlog->LogError("status %s", u_errorName(status));
		return;
	}

    transIDs = ures_getByKey(bundle, SW_RB_RULE_BASED_IDS, 0, &status);
	UParseError parseError;

    int32_t row, maxRows;
    if (U_SUCCESS(status)) {
	   maxRows = ures_getSize(transIDs);
	   for (row = 0; row < maxRows; row++) {
		  colBund = ures_getByIndex(transIDs, row, 0, &status);

		  if (U_SUCCESS(status) && ures_getSize(colBund) == 4) {
			 UnicodeString id = ures_getUnicodeStringByIndex(colBund, 0, &status);
			 UChar type = ures_getUnicodeStringByIndex(colBund, 1, &status).charAt(0);
			 UnicodeString resString = ures_getUnicodeStringByIndex(colBund, 2, &status);
				SWLog::systemlog->LogInformation("ok so far");

			 if (U_SUCCESS(status)) {
				switch (type) {
				case 0x66: // 'f'
				case 0x69: // 'i'
				    // 'file' or 'internal';
				    // row[2]=resource, row[3]=direction
				    {
					   UBool visible = (type == 0x0066 /*f*/);
					   UTransDirection dir =
						  (ures_getUnicodeStringByIndex(colBund, 3, &status).charAt(0) ==
						   0x0046 /*F*/) ?
						  UTRANS_FORWARD : UTRANS_REVERSE;
					   //registry->put(id, resString, dir, visible);
			    //SWLog::systemlog->LogInformation("instantiating %s ...", (const char *)(UnicodeCaster)resString);
				SWLog::systemlog->LogInformation("instantiating %s ...", resString.getBuffer());
			    registerTrans(id, resString, dir, status);
				SWLog::systemlog->LogInformation("done.");
				    }
				    break;
				case 0x61: // 'a'
				    // 'alias'; row[2]=createInstance argument
				    //registry->put(id, resString, TRUE);
				    break;
				}
			 }
		else SWLog::systemlog->LogError("Failed to get resString");
		  }
	    else SWLog::systemlog->LogError("Failed to get row");

		  ures_close(colBund);
	   }
    }
	else
	{
		SWLog::systemlog->LogError("no resource index to load");
		SWLog::systemlog->LogError("status %s", u_errorName(status));
	}

    ures_close(transIDs);
    ures_close(bundle);
	
#endif // _ICUSWORD_
}

void  UTF8Transliterator::registerTrans(const UnicodeString& ID, const UnicodeString& resource,
		UTransDirection dir, UErrorCode &status )
{
#ifndef _ICUSWORD_
		//SWLog::systemlog->LogInformation("registering ID locally %s", (const char *)(UnicodeCaster)ID);
		SWLog::systemlog->LogInformation("registering ID locally %s", ID.getBuffer());
		SWTransData swstuff; 
		swstuff.resource = resource;
		swstuff.dir = dir;
		SWTransPair swpair;
		swpair.first = ID;
		swpair.second = swstuff;
		transMap.insert(swpair);
#endif
}

bool UTF8Transliterator::checkTrans(const UnicodeString& ID, UErrorCode &status )
{
#ifndef _ICUSWORD_
		Transliterator *trans = Transliterator::createInstance(ID, UTRANS_FORWARD, status);
		if (!U_FAILURE(status))
		{
			// already have it, clean up and return true
			//SWLog::systemlog->LogInformation("already have it %s", (const char *)(UnicodeCaster)ID);
			SWLog::systemlog->LogInformation("already have it %s", ID.getBuffer());
			delete trans;
			return true;
		}
		status = U_ZERO_ERROR;
	
	SWTransMap::iterator swelement;
	if ((swelement = transMap.find(ID)) != transMap.end())
	{
		SWLog::systemlog->LogInformation("found element in map");
		SWTransData swstuff = (*swelement).second;
		UParseError parseError;
		//UErrorCode status;
		//std::cout << "unregistering " << ID << std::endl;
		//Transliterator::unregister(ID);
		//SWLog::systemlog->LogInformation("resource is %s", (const char *)(UnicodeCaster)swstuff.resource);
		SWLog::systemlog->LogInformation("resource is %s", swstuff.resource.getBuffer());
		
		// Get the rules
		//std::cout << "importing: " << ID << ", " << resource << std::endl;
		SWCharString ch(swstuff.resource);
		UResourceBundle *bundle = ures_openDirect(SW_RESDATA, ch, &status);
		const UnicodeString rules = ures_getUnicodeStringByKey(bundle, SW_RB_RULE, &status);
		ures_close(bundle);
		//parser.parse(rules, isReverse ? UTRANS_REVERSE : UTRANS_FORWARD,
		//        parseError, status);
		if (U_FAILURE(status)) {
			SWLog::systemlog->LogError("Failed to get rules");
			SWLog::systemlog->LogError("status %s", u_errorName(status));
			return false;
		}

		
		Transliterator *trans = Transliterator::createFromRules(ID, rules, swstuff.dir,
			parseError,status);
		if (U_FAILURE(status)) {
			SWLog::systemlog->LogError("Failed to create transliterator");
			SWLog::systemlog->LogError("status %s", u_errorName(status));
			SWLog::systemlog->LogError("Parse error: line %s", parseError.line);
			SWLog::systemlog->LogError("Parse error: offset %d", parseError.offset);
			SWLog::systemlog->LogError("Parse error: preContext %s", *parseError.preContext);
			SWLog::systemlog->LogError("Parse error: postContext %s", *parseError.postContext);
			SWLog::systemlog->LogError("rules were");
//			SWLog::systemlog->LogError((const char *)rules);
			return false;
		}

		Transliterator::registerInstance(trans);
		return true;
		
		//Transliterator *trans = instantiateTrans(ID, swstuff.resource, swstuff.dir, parseError, status);
		//return trans;
	}
	else
	{
		return false;
	}
#else
return true;
#endif // _ICUSWORD_
}

Transliterator * UTF8Transliterator::createTrans(const UnicodeString& preID, const UnicodeString& ID, 
	const UnicodeString& postID, UTransDirection dir, UErrorCode &status )
{
	// extract id to check from ID xxx;id;xxx
	if (checkTrans(ID, status)) {
		UnicodeString fullID = preID;
		fullID += ID;
		fullID += postID;
		Transliterator *trans = Transliterator::createInstance(fullID,UTRANS_FORWARD,status);
		if (U_FAILURE(status)) {
			delete trans;
			return NULL;
		}
		else {
			return trans;
		}
	}
	else {
		return NULL;
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

char UTF8Transliterator::processText(SWBuf &text, const SWKey *key, const SWModule *module)
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

                UnicodeString preid;
                if (compat) {
                        preid = UnicodeString("NFKD;");
                }
                else {
                        preid = UnicodeString("NFD;");
                }

		//Simple X to Latin transliterators
		UnicodeString id;
		if (scripts[SE_GREEK]) {
			if (option == SE_BETA)
				id = UnicodeString("Greek-Beta");
			else if (option == SE_BGREEK)
				id = UnicodeString("Greek-BGreek");
			else {
	            if (!strnicmp (((SWModule*)module)->Lang(), "cop", 3)) {
        				id = UnicodeString("Coptic-Latin");
                }
                else {
		   				id = UnicodeString("Greek-Latin");
                }
				scripts[SE_LATIN] = true;
			}
		}
		if (scripts[SE_HEBREW]) {
			if (option == SE_BETA)
				id = UnicodeString("Hebrew-CCAT");
                        else if (option == SE_SYRIAC)
                                id = UnicodeString("Hebrew-Syriac");
			else {
				id = UnicodeString("Hebrew-Latin");
				scripts[SE_LATIN] = true;
			}
		}
		if (scripts[SE_CYRILLIC]) {
			id = UnicodeString("Cyrillic-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_ARABIC]) {
			id = UnicodeString("Arabic-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_SYRIAC]) {
                        if (option == SE_BETA)
        			id = UnicodeString("Syriac-CCAT");
                        else if (option == SE_HEBREW)
                                id = UnicodeString("Syriac-Hebrew");
                        else {
        			id = UnicodeString("Syriac-Latin");
        			scripts[SE_LATIN] = true;
                        }
		}
		if (scripts[SE_THAI]) {
			id = UnicodeString("Thai-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_GEORGIAN]) {
			id = UnicodeString("Georgian-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_ARMENIAN]) {
			id = UnicodeString("Armenian-Latin");
			scripts[SE_LATIN] = true;
		}                
		if (scripts[SE_ETHIOPIC]) {
			id = UnicodeString("Ethiopic-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_GOTHIC]) {
			id = UnicodeString("Gothic-Latin");
			scripts[SE_LATIN] = true;
		}
		if (scripts[SE_UGARITIC]) {
			id = UnicodeString("Ugaritic-Latin");
			scripts[SE_LATIN] = true;
		}
        if (scripts[SE_HAN]) {
	        if (!strnicmp (((SWModule*)module)->Lang(), "ja", 2)) {
     			id = UnicodeString("Kanji-OnRomaji");
            }
            else {
       			id = UnicodeString("Han-Pinyin");
            }
			scripts[SE_LATIN] = true;
		}

       		// Inter-Kana and Kana to Latin transliterators
		if (option == SE_HIRAGANA && scripts[SE_KATAKANA]) {
			id = UnicodeString("Katakana-Hiragana");
			scripts[SE_HIRAGANA] = true;
		}
		else if (option == SE_KATAKANA && scripts[SE_HIRAGANA]) {
			id = UnicodeString("Hiragana-Katakana");
			scripts[SE_KATAKANA] = true;
		}
		else {
        		if (scripts[SE_KATAKANA]) {
	        		id = UnicodeString("Katakana-Latin");
		        	scripts[SE_LATIN] = true;
        		}
	        	if (scripts[SE_HIRAGANA]) {
		        	id = UnicodeString("Hiragana-Latin");
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
				id = UnicodeString("Hangul-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_JAMO]) {
				id = UnicodeString("Jamo-Latin");
				scripts[SE_LATIN] = true;
			}
		}

		// Indic-Latin
		if (option < SE_DEVANAGARI || option > SE_MALAYALAM) {
			// Indic to Latin
			if (scripts[SE_TAMIL]) {
				id = UnicodeString("Tamil-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_BENGALI]) {
				id = UnicodeString("Bengali-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_GURMUKHI]) {
				id = UnicodeString("Gurmukhi-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_GUJARATI]) {
				id = UnicodeString("Gujarati-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_ORIYA]) {
				id = UnicodeString("Oriya-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_TELUGU]) {
				id = UnicodeString("Telugu-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_KANNADA]) {
				id = UnicodeString("Kannada-Latin");
				scripts[SE_LATIN] = true;
			}
			if (scripts[SE_MALAYALAM]) {
				id = UnicodeString("Malayalam-Latin");
				scripts[SE_LATIN] = true;
			}
		}
		else {
			if (scripts[SE_LATIN]) {
				id = UnicodeString("Latin-InterIndic");
			}
			if (scripts[SE_DEVANAGARI]) {
				id = UnicodeString("Devanagari-InterIndic");
			}
			if (scripts[SE_TAMIL]) {
				id = UnicodeString("Tamil-InterIndic");
			}
			if (scripts[SE_BENGALI]) {
				id = UnicodeString("Bengali-InterIndic");
			}
			if (scripts[SE_GURMUKHI]) {
				id = UnicodeString("Gurmurkhi-InterIndic");
			}
			if (scripts[SE_GUJARATI]) {
				id = UnicodeString("Gujarati-InterIndic");
			}
			if (scripts[SE_ORIYA]) {
				id = UnicodeString("Oriya-InterIndic");
			}
			if (scripts[SE_TELUGU]) {
				id = UnicodeString("Telugu-InterIndic");
			}
			if (scripts[SE_KANNADA]) {
				id = UnicodeString("Kannada-InterIndic");
			}
			if (scripts[SE_MALAYALAM]) {
				id = UnicodeString("Malayalam-InterIndic");
			}

			switch(option) {
			case SE_DEVANAGARI:
				id = UnicodeString("InterIndic-Devanagari");
				break;
			case SE_TAMIL:
				id = UnicodeString("InterIndic-Tamil");
				break;
			case SE_BENGALI:
				id = UnicodeString("InterIndic-Bengali");
				break;
			case SE_GURMUKHI:
				id = UnicodeString("InterIndic-Gurmukhi");
				break;
			case SE_GUJARATI:
				id = UnicodeString("InterIndic-Gujarati");
				break;
			case SE_ORIYA:
				id = UnicodeString("InterIndic-Oriya");
				break;
			case SE_TELUGU:
				id = UnicodeString("InterIndic-Telugu");
				break;
			case SE_KANNADA:
				id = UnicodeString("InterIndic-Kannada");
				break;
			case SE_MALAYALAM:
				id = UnicodeString("InterIndic-Malayalam");
				break;
			default:
				id = UnicodeString("InterIndic-Latin");
				scripts[SE_LATIN] = true;
				break;
			}
		}

		if (scripts[SE_LATIN]) {
                switch (option) {
                        case SE_GREEK:
				id = UnicodeString("Latin-Greek");
                                break;
                        case SE_HEBREW:
				id = UnicodeString("Latin-Hebrew");
                                break;
                        case SE_CYRILLIC:
				id = UnicodeString("Latin-Cyrillic");
                                break;
                        case SE_ARABIC:
				id = UnicodeString("Latin-Arabic");
                                break;
                        case SE_SYRIAC:
				id = UnicodeString("Latin-Syriac");
                                break;
                        case SE_THAI:
				id = UnicodeString("Latin-Thai");
                                break;
                        case SE_GEORGIAN:
				id = UnicodeString("Latin-Georgian");
                                break;
                        case SE_ARMENIAN:
				id = UnicodeString("Latin-Armenian");
                                break;
                        case SE_ETHIOPIC:
				id = UnicodeString("Latin-Ethiopic");
                                break;
                        case SE_GOTHIC:
				id = UnicodeString("Latin-Gothic");
                                break;
                        case SE_UGARITIC:
				id = UnicodeString("Latin-Ugaritic");
                                break;
                        case SE_COPTIC:
				id = UnicodeString("Latin-Coptic");
                                break;
                        case SE_KATAKANA:
				id = UnicodeString("Latin-Katakana");
                                break;
                        case SE_HIRAGANA:
				id = UnicodeString("Latin-Hiragana");
                                break;
                        case SE_JAMO:
				id = UnicodeString("Latin-Jamo");
                                break;
                        case SE_HANGUL:
				id = UnicodeString("Latin-Hangul");
                                break;
                        }
                }

                if (option == SE_BASICLATIN) {
                        id = UnicodeString("Any-Latin1");
                }
                UnicodeString postid;                
                if (noNFC) {
                        postid = UnicodeString(";NFD");
                } else {
                        postid = UnicodeString(";NFC");
                }

                //UParseError perr;

                err = U_ZERO_ERROR;
                //Transliterator * trans = Transliterator::createInstance(id, UTRANS_FORWARD, perr, err);
				Transliterator * trans = createTrans(preid, id, postid, UTRANS_FORWARD, err);
                if (trans && !U_FAILURE(err)) {
                        UnicodeString target = UnicodeString(source);
				    trans->transliterate(target);
				    text.setSize(text.size()*2);
				    len = ucnv_fromUChars(conv, text.getRawData(), text.size(), target.getBuffer(), target.length(), &err);
					text.setSize(len);
				    delete trans;
                }
                ucnv_close(conv);
        }
	return 0;
}

SWORD_NAMESPACE_END
#endif



