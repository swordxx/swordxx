/******************************************************************************
 *
 * utf8transliterators - SWFilter decendant to transliterate between
 *			 ICU-supported scripts.
 */

#ifdef ICU

#include <stdlib.h>
#include <string.h>

#ifndef __GNUC__
#else
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
  "Malayalam"
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
  for (unsigned char i = NUMSCRIPTS; i && stricmp(ival, optionstring[i]); i--)
    option = i;
}

const char *UTF8Transliterator::getOptionValue()
{
  return (NUMSCRIPTS >= option) ? optionstring[option] : 0;
}

char UTF8Transliterator::ProcessText(char *text, int maxlen, const SWKey *key)
{
	if (!option) {	// if we don't want transliteration
		char *to, *from; // cheese.  Fix.
		int len;

		len = strlen(text) + 1;

		if (len < maxlen) {
			memmove(&text[maxlen - len], text, len);
			from = &text[maxlen - len];
		}
		else
		        from = text;	
		to = text;
		// -------------------------------

		// Convert UTF-8 string to UTF-16 (UChars)
		int32_t ulen;
		UErrorCode status = U_ZERO_ERROR;
		UnicodeConverter conv("utf-8", status);
		UnicodeString ustr;
		conv.toUnicodeString(ustr, text, len, status);
		ulen = ustr.length();

		// Figure out which scripts are used in the string
		unsigned char i, j;
		unsigned char scripts[NUMSCRIPTS];
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
		  default: scripts[SE_LATIN] = true;
		  }
		} 
		scripts[option] = false; //turn off the reflexive transliteration
		
		//return if we have no transliteration to do for this text
		for (i = 0; i < NUMSCRIPTS; i++) {
		  if (scripts[i]) j++;
		}
		if (!j) return 0;

		Transliterator* trans;
		UParseError perror;
		//Simple X to Latin transliterators
		if (scripts[SE_GREEK]) {
		  trans = Transliterator::createInstance("Greek-Latin", UTRANS_FORWARD, &perror);
		  trans->transliterate(ustr);
		  delete trans;
		  scripts[SE_LATIN] = true;
		}
		if (scripts[SE_HEBREW]) {
		  trans = Transliterator::createInstance("Hebrew-Latin", UTRANS_FORWARD, &perror);
		  trans->transliterate(ustr);
		  delete trans;
		  scripts[SE_LATIN] = true;
		}
		if (scripts[SE_CYRILLIC]) {
		  trans = Transliterator::createInstance("Cyrillic-Latin", UTRANS_FORWARD, &perror);
		  trans->transliterate(ustr);
		  delete trans;
		  scripts[SE_LATIN] = true;
		}
		if (scripts[SE_ARABIC]) {
		  trans = Transliterator::createInstance("Arabic-Latin", UTRANS_FORWARD, &perror);
		  trans->transliterate(ustr);
		  delete trans;
		  scripts[SE_LATIN] = true;
		}
		if (scripts[SE_KANA]) {
		  trans = Transliterator::createInstance("Kana-Latin", UTRANS_FORWARD, &perror);
		  trans->transliterate(ustr);
		  delete trans;
		  scripts[SE_LATIN] = true;
		}


		// Inter-Korean and Korean to Latin transliterators
		if (option == SE_HANGUL && scripts[SE_JAMO]) {
		  trans = Transliterator::createInstance("Jamo-Hangul", UTRANS_FORWARD, &perror);
		  trans->transliterate(ustr);
		  delete trans;
		  scripts[SE_HANGUL] = true;
		}
		else if (option == SE_JAMO && scripts[SE_HANGUL]) {
		  trans = Transliterator::createInstance("Hangul-Jamo", UTRANS_FORWARD, &perror);
		  trans->transliterate(ustr);
		  delete trans;
		  scripts[SE_JAMO] = true;
		}
		else {
		  if (scripts[SE_HANGUL]) {
		    trans = Transliterator::createInstance("Hangul-Jamo", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_JAMO] = true;
		  }
		  if (scripts[SE_JAMO]) {
		    trans = Transliterator::createInstance("Jamo-Latin", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_LATIN] = true;
		  }
		}
		
		// Indic-Latin
		if (option < SE_DEVANAGARI) {
		  // first converter Indic to Devanagari
		  if (scripts[SE_TAMIL]) {
		    trans = Transliterator::createInstance("Tamil-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_DEVANAGARI] = true;
		  }
		  if (scripts[SE_BENGALI]) {
		    trans = Transliterator::createInstance("Bengali-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_DEVANAGARI] = true;
		  }
		  if (scripts[SE_GURMUKHI]) {
		    trans = Transliterator::createInstance("Gurmukhi-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_DEVANAGARI] = true;
		  }
		  if (scripts[SE_GUJARATI]) {
		    trans = Transliterator::createInstance("Gujarati-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_DEVANAGARI] = true;
		  }
		  if (scripts[SE_ORIYA]) {
		    trans = Transliterator::createInstance("Oriya-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_DEVANAGARI] = true;
		  }
		  if (scripts[SE_TELUGU]) {
		    trans = Transliterator::createInstance("Telugu-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_DEVANAGARI] = true;
		  }
		  if (scripts[SE_KANNADA]) {
		    trans = Transliterator::createInstance("Kannada-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_DEVANAGARI] = true;
		  }
		  if (scripts[SE_MALAYALAM]) {
		    trans = Transliterator::createInstance("Malayalam-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_DEVANAGARI] = true;
		  }
		  
		  if (scripts[SE_DEVANAGARI]) {
		    trans = Transliterator::createInstance("Devanagari-Latin", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_LATIN] = true;
		  }
		}
		else {
		  if (scripts[SE_LATIN]) {
		    trans = Transliterator::createInstance("Latin-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    scripts[SE_DEVANAGARI] = true;		    
		  }

		  if (scripts[SE_TAMIL]) {
		    trans = Transliterator::createInstance("Tamil-InterIndic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (scripts[SE_BENGALI]) {
		    trans = Transliterator::createInstance("Bengali-InterIndic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (scripts[SE_GURMUKHI]) {
		    trans = Transliterator::createInstance("Gurmurkhi-InterIndic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (scripts[SE_GUJARATI]) {
		    trans = Transliterator::createInstance("Gujarati-InterIndic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (scripts[SE_ORIYA]) {
		    trans = Transliterator::createInstance("Oriya-InterIndic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (scripts[SE_TELUGU]) {
		    trans = Transliterator::createInstance("Telugu-InterIndic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (scripts[SE_KANNADA]) {
		    trans = Transliterator::createInstance("Kannada-InterIndic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (scripts[SE_MALAYALAM]) {
		    trans = Transliterator::createInstance("Malayalam-InterIndic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (scripts[SE_DEVANAGARI]) {
		    trans = Transliterator::createInstance("Devanagari-InterIndic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }

		  switch(option) {
		  case SE_TAMIL: 
		    trans = Transliterator::createInstance("InterIndic-Tamil", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    break;
		  case SE_BENGALI: 
		    trans = Transliterator::createInstance("InterIndic-Bengali", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    break;
		  case SE_GURMUKHI: 
		    trans = Transliterator::createInstance("InterIndic-Gurmukhi", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    break;
		  case SE_GUJARATI: 
		    trans = Transliterator::createInstance("InterIndic-Gujarati", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    break;
		  case SE_ORIYA: 
		    trans = Transliterator::createInstance("InterIndic-Oriya", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    break;
		  case SE_TELUGU: 
		    trans = Transliterator::createInstance("InterIndic-Telugu", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    break;
		  case SE_KANNADA: 
		    trans = Transliterator::createInstance("InterIndic-Kannada", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    break;
		  case SE_MALAYALAM: 
		    trans = Transliterator::createInstance("InterIndic-Malayalam", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    break;
		  case SE_DEVANAGARI: 
		    trans = Transliterator::createInstance("InterIndic-Devanagari", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    break;
		  }
		}

		if (scripts[SE_LATIN]) {
		  if (option == SE_GREEK) {
		    trans = Transliterator::createInstance("Latin-Greek", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (option == SE_HEBREW) {
		    trans = Transliterator::createInstance("Latin-Hebrew", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (option == SE_CYRILLIC) {
		    trans = Transliterator::createInstance("Latin-Cyrillic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (option == SE_ARABIC) {
		    trans = Transliterator::createInstance("Latin-Arabic", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (option == SE_KANA) {
		    trans = Transliterator::createInstance("Latin-Kana", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (option == SE_JAMO) {
		    trans = Transliterator::createInstance("Latin-Jamo", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		  if (option == SE_HANGUL) {
		    trans = Transliterator::createInstance("Latin-Jamo", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		    trans = Transliterator::createInstance("Jamo-Hangul", UTRANS_FORWARD, &perror);
		    trans->transliterate(ustr);
		    delete trans;
		  }
		}
		int32_t newlen = ((unsigned int)maxlen) > sizeof(from) ? sizeof(from) : maxlen;
		conv.fromUnicodeString(from, newlen, ustr, status);
	}
	return 0;
}
#endif