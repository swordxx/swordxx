// Diatheke 4.1 by Chris Little <chrislit@crosswire.org>
// Copyright 1999, 2000, 2001 by CrossWire Bible Society
// http://www.crosswire.org/sword/diatheke
// Licensed under GNU General Public License (GPL)
// see accompanying LICENSE file for license details

#include "corediatheke.h"
#include <iostream>
#include <string>
#include <list>

void systemquery(const char * key, ostream* output){
	SWMarkupMgr manager;
	ModMap::iterator it;

	SWModule *target;
	
	bool types = false, descriptions = false, names = false;

	if (!stricmp(key, "localelist")) {		
		static LocaleMgr lm = LocaleMgr::systemLocaleMgr;
		list<string> loclist =	lm.getAvailableLocales();
		list<string>::iterator li = loclist.begin();
		for (;li != loclist.end(); li++) {
		  *output << li->c_str();
		  *output << "\n";
		}
	}
	else if (!stricmp(key, "modulelist")) {
		types = true;
		descriptions = true;
		names = true;
	}
	else if (!stricmp(key, "modulelistnames")) {
		names = true;
	}
	else if (!stricmp(key, "modulelistdescriptions")) {
		descriptions = true;
	}
	
	
	if (types || descriptions || names) {
		if (types) *output << "Biblical Texts:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->Type(), "Biblical Texts")) {
				if (names) *output << target->Name();
				if (names && descriptions) *output << " : ";
				if (descriptions) *output << target->Description();
				*output << "\n";
			}
		}
		if (types) *output << "Commentaries:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->Type(), "Commentaries")) {
				if (names) *output << target->Name();
				if (names && descriptions) *output << " : ";
				if (descriptions) *output << target->Description();
				*output << "\n";
			} 
		}
		if (types) *output << "Dictionaries:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->Type(), "Lexicons / Dictionaries")) {
				if (names) *output << target->Name();
				if (names && descriptions) *output << " : ";
				if (descriptions) *output << target->Description();
				*output << "\n";
			}
		}
	}
}

void doquery(unsigned long maxverses = -1, unsigned char outputformat = FMT_PLAIN, unsigned char outputencoding = ENC_UTF8, unsigned long optionfilters = 0, unsigned char searchtype = ST_NONE, const char *text = 0, const char *locale = 0, const char *ref = 0, ostream* output = &cout, const char *script = 0) { 
	static DiathekeMgr manager;

	ModMap::iterator it;
	ListKey listkey;
	SectionMap::iterator sit;
	ConfigEntMap::iterator eit;
	
	SWModule * target;
	char *font = 0;
	char inputformat = 0;
	string encoding;
	char querytype = 0;	

	if (locale) {
		LocaleMgr::systemLocaleMgr.setDefaultLocaleName(locale);
	}
	VerseKey vk;

	//deal with queries to "system"
	if (!stricmp(text, "system")) {
		querytype = QT_SYSTEM;
		systemquery(ref, output);
	}
	if (!strnicmp(text, "info", 4)) {
	        querytype = QT_INFO;
		text = ref;
	}
	//otherwise, we have a real book
	it = manager.Modules.find(text);
	if (it == manager.Modules.end()) { //book not found
		return;
	}
	target = (*it).second;

	manager.Markup(outputformat);
	manager.Encoding(outputencoding);
	manager.bidi = ((OP_BIDI & optionfilters) == OP_BIDI);
	manager.shape = ((OP_ARSHAPE & optionfilters) == OP_ARSHAPE);
	
	if ((sit = manager.config->Sections.find((*it).second->Name())) != manager.config->Sections.end()) {
		if ((eit = (*sit).second.find("SourceType")) != (*sit).second.end()) {
			if (!stricmp((char *)(*eit).second.c_str(), "GBF"))
				inputformat = FMT_GBF;
			else if (!stricmp((char *)(*eit).second.c_str(), "ThML"))
				inputformat = FMT_THML;
		}
		encoding = ((eit = (*sit).second.find("Encoding")) != (*sit).second.end()) ? (*eit).second : (string)"";
	}


	if (querytype == QT_INFO) {
	  switch (inputformat) {
	  case FMT_THML :
	    *output << "ThML";
	    break;
	  case FMT_GBF :
	    *output << "GBF";
	    break;
	  default:
	    *output << "Other";
	  }	 
	  *output << ";";
	  *output << target->Type();
	  *output << ";";
	  return;
	}

	if (searchtype)
		querytype = QT_SEARCH;
	else if (!strcmp(target->Type(), "Biblical Texts"))
		querytype = QT_BIBLE;
	else if (!strcmp(target->Type(), "Commentaries"))
		querytype = QT_COMM;
	else if (!strcmp(target->Type(), "Lexicons / Dictionaries"))
		querytype = QT_LD;
	
	if (optionfilters & OP_FOOTNOTES)
		manager.setGlobalOption("Footnotes","On");
	else
		manager.setGlobalOption("Footnotes","Off");
	if (optionfilters & OP_HEADINGS)
		manager.setGlobalOption("Headings","On");
	else
		manager.setGlobalOption("Headings","Off");
	if (optionfilters & OP_STRONGS)
		manager.setGlobalOption("Strong's Numbers","On");
	else
		manager.setGlobalOption("Strong's Numbers","Off");
	if (optionfilters & OP_MORPH)
		manager.setGlobalOption("Morphological Tags","On");
	else
		manager.setGlobalOption("Morphological Tags","Off");
	if (optionfilters & OP_CANTILLATION)
		manager.setGlobalOption("Hebrew Cantillation","On");
	else
		manager.setGlobalOption("Hebrew Cantillation","Off");
	if (optionfilters & OP_HEBREWPOINTS)
		manager.setGlobalOption("Hebrew Vowel Points","On");
	else
		manager.setGlobalOption("Hebrew Vowel Points","Off");
	if (optionfilters & OP_GREEKACCENTS)
		manager.setGlobalOption("Greek Accents","On");
	else
		manager.setGlobalOption("Greek Accents","Off");
	if (optionfilters & OP_LEMMAS)
		manager.setGlobalOption("Lemmas","On");
	else
		manager.setGlobalOption("Lemmas","Off");
	if (optionfilters & OP_SCRIPREFS)
		manager.setGlobalOption("Scripture Cross-References","On");
	else
		manager.setGlobalOption("Scripture Cross-References","Off");

#ifdef _ICU_
	if (optionfilters & OP_TRANSLITERATOR && script)
                manager.setGlobalOption("Transliteration", script);
	else
		manager.setGlobalOption("Transliteration", "Off");
#endif
	
	if (querytype == QT_SEARCH) {
		//do search stuff
		searchtype = 1 - searchtype;
		*output << "Verse(s) containing \"";
	        *output << ref;
		*output << "\"-- ";
		
		listkey = target->Search(ref, searchtype);
		
		if (strlen((const char*)listkey)) {
		  if (!listkey.Error()) {
		    vk = listkey;
		    *output << (const char *)vk;
		  }
		  listkey++;
		  while (!listkey.Error()) {
		    *output << " ; ";
		    vk = listkey;
		    *output << (const char *)vk;
		    listkey++;
		  }
		  *output << " -- ";
		  
		  char *temp = new char[10];
		  sprintf(temp, "%u", listkey.Count());
		  *output << temp;
		  delete [] temp;
		  
		  *output << " matches total (";
		  *output << target->Name();
		  *output << ")\n";
		}
		else {
		  *output << "none (";
		  *output << target->Name();
		  *output << ")\n";
		}
	}
	
	else if (querytype == QT_LD) {
		//do dictionary stuff
		
		target->SetKey(ref);
		
		char * text = (char *) *target;

		if (outputformat == FMT_RTF) {
			*output << "{\\rtf1\\ansi{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f1\\fdecor\\fprq2 ";
			if (font)
				*output << font;
			else
				*output << "Times New Roman";
			*output << ";}}";
		}
		else if (outputformat == FMT_HTML) {
			*output << "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">";
		}		
	
		if (strlen(text)) {
			*output << (char*)target->KeyText();
			if (font && (outputformat == FMT_HTML || outputformat == FMT_THML)) {
				*output << ": <font face=\"";
				*output << font;
				*output << "\">";
			}
			else if (outputformat == FMT_RTF) {
				*output << ": {\\f1 ";
			}
			else {
				*output << ": ";
			}
			*output << text;
			if (font && (outputformat == FMT_HTML || outputformat == FMT_THML)) {
				*output << "</font>";
			}
			else if (outputformat == FMT_RTF) {
				*output << "}";
			}

			*output << "(";
			*output << target->Name();
			*output << ")\n";
		}	

		if (outputformat == FMT_RTF) {
			*output << "}";
		}

	}
	
	else if (querytype == QT_BIBLE || querytype == QT_COMM) {
		//do commentary/Bible stuff

		if ((sit = manager.config->Sections.find((*it).second->Name())) != manager.config->Sections.end()) {
			if ((eit = (*sit).second.find("Font")) != (*sit).second.end()) {
				font = (char *)(*eit).second.c_str();
				if (strlen(font) == 0) font = 0;
			}
		}
		
 		listkey = vk.ParseVerseList(ref, "Gen1:1", true);
		int i;

		if (outputformat == FMT_RTF) {
			*output << "{\\rtf1\\ansi{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f1\\fdecor\\fprq2 ";
			if (font)
				*output << font;
			else
				*output << "Times New Roman";
			*output << ";}{\\f7\\froman\\fcharset2\\fprq2 Symbol;}}";
		}
		else if (outputformat == FMT_HTML) {
			*output << "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">";
		}

		for (i = 0; i < listkey.Count() && maxverses; i++) {
			VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.GetElement(i));
			if (element) {
			  target->Key(element->LowerBound());
				vk = element->UpperBound();
				while (maxverses && target->Key() <= vk) {
					*output << (char*)target->KeyText();
					if (font && (outputformat == FMT_HTML || outputformat == FMT_THML)) {
						*output << ": <font face=\"";
						*output << font;
						*output << "\">";
					}
					else if (outputformat == FMT_RTF) {
						*output << ": {\\f1 ";
					}
					else {
						*output << ": ";
					}
					*output << (const char*)*target;
					if (font && (outputformat == FMT_HTML || outputformat == FMT_THML)) {
						*output << "</font>";
					}
					else if (outputformat == FMT_RTF) {
						*output << "}";
					}

					if (inputformat != FMT_THML && (outputformat == FMT_HTML || outputformat == FMT_THML))
						*output << "<br />";
					else if (outputformat == FMT_RTF)
						*output << "\\par ";
					else if (outputformat == FMT_GBF)
						*output << "<CM>";

					*output << "\n";
					
					if (target->Key() == vk)
					  break;
					maxverses--;
					(*target)++;
				}
			}
			else {
				target->Key(*listkey.GetElement(i));
				*output << (char*)target->KeyText();
				if (font && (outputformat == FMT_HTML || outputformat == FMT_THML)) {
					*output << ": <font face=\"";
					*output << font;
					*output << "\">";
				}
				else if (outputformat == FMT_RTF) {
					*output << ": {\\f1 ";
				}
				else {
					*output << ": ";
				}
				*output << (const char*)*target;
				if (font && (outputformat == FMT_HTML || outputformat == FMT_THML)) {
					*output << "</font>";
				}
				else if (outputformat == FMT_RTF) {
					*output << "}";
				}
					
				if (inputformat != FMT_THML && (outputformat == FMT_HTML || outputformat == FMT_THML))
					*output << "<br />";
				else if (outputformat == FMT_RTF)
					*output << "\\par ";
				else if (outputformat == FMT_GBF)
					*output << "<CM>";

				*output << "\n";
				maxverses--;
			}
		}

		*output << "(";
		*output << target->Name();
		*output << ")\n";

		if (outputformat == FMT_RTF) {
			*output << "}";
		}

	}
}

