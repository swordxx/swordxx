// Diatheke 4.0 by Chris Little <chrislit@crosswire.org>
// Copyright 1999, 2000, 2001 by CrossWire Bible Society http://www.crosswire.org
// Licensed under GNU General Public License (GPL)
// see accompanying LICENSE file for license details
#include "corediatheke.h"
#include <string>
#include <list>
extern "C" {
#include <roman.h>
}

char * systemquery(const char * key){
	SWMgr manager;
	ModMap::iterator it;

	SWModule *target;
	string value;
	
	bool types = false, descriptions = false, names = false;

	if (!stricmp(key, "localelist")) {		
		LocaleMgr lm = LocaleMgr::systemLocaleMgr;
		list<string> loclist =	lm.getAvailableLocales();
		list<string>::iterator li = loclist.begin();
		for (;li != loclist.end(); li++) {
			value += li->c_str();
			value += "\n";
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
		if (types) value += "Biblical Texts:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->Type(), "Biblical Texts")) {
				if (names) value += target->Name();
				if (names && descriptions) value += " : ";
				if (descriptions) value += target->Description();
				value += "\n";
			}
		}
		if (types) value += "Commentaries:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->Type(), "Commentaries")) {
				if (names) value += target->Name();
				if (names && descriptions) value += " : ";
				if (descriptions) value += target->Description();
				value += "\n";
			} 
		}
		if (types) value += "Dictionaries:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->Type(), "Lexicons / Dictionaries")) {
				if (names) value += target->Name();
				if (names && descriptions) value += " : ";
				if (descriptions) value += target->Description();
				value += "\n";
			}
		}
	}

	char * versevalue = new char[value.length() + 1];
	strcpy (versevalue, value.c_str());
	return versevalue;
}

char* doquery(int maxverses = -1, char outputformat = FMT_PLAIN, char optionfilters = 0, char searchtype = ST_NONE, const char *text = 0, const char *locale = 0, const char *ref = 0) { 
	static SWMgr manager;
	static ModMap::iterator it;
	static ListKey listkey;
	static SectionMap::iterator sit;
	static ConfigEntMap::iterator eit;
	static VerseKey vk;
	
	SWModule * target;
	SWFilter * filter = 0;
	SWFilter * gbffilter = 0;
	SWFilter * chfilter = 0;
	char *font = 0;
	char inputformat = 0;
	string value = "";
	string encoding;
	char querytype = 0;	

	//char * ref2 = new char[strlen(ref)];
	//strcpy(ref2, ref);

	//deal with queries to "system"
	if (!stricmp(text, "system")) {
		querytype = QT_SYSTEM;
		char * ret = systemquery(ref);
		
		return ret;
	}
	if (!strnicmp(text, "info", 4)) {
	        querytype = QT_INFO;
		text = ref;
	}
	//otherwise, we have a real book
	it = manager.Modules.find(text);
	if (it == manager.Modules.end()) { //book not found
		
		return NULL;
	}

	target = (*it).second;
	
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
	    value += "ThML";
	    break;
	  case FMT_GBF :
	    value += "GBF";
	    break;
	  default:
	    value += "Other";
	  }	 
	  value += ";";
	  value += target->Type();
	  
	  char * versevalue = new char[value.length() + 1];
	  strcpy (versevalue, value.c_str());
	  
	  return versevalue;
	}

	if (searchtype)
		querytype = QT_SEARCH;
	else if (!strcmp(target->Type(), "Biblical Texts"))
		querytype = QT_BIBLE;
	else if (!strcmp(target->Type(), "Commentaries"))
		querytype = QT_COMM;
	else if (!strcmp(target->Type(), "Lexicons / Dictionaries"))
		querytype = QT_LD;

	if (locale) {
		LocaleMgr::systemLocaleMgr.setDefaultLocaleName(locale);
	}
	
	if (outputformat == FMT_PLAIN)
		filter = new ThMLPlain();
	else if (outputformat == FMT_HTML)
		filter = new ThMLHTML();
	else if (outputformat == FMT_GBF)
		filter = new ThMLGBF();
	else if (outputformat == FMT_RTF)
		filter = new ThMLRTF();
	
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
	
	if (querytype == QT_SEARCH) {
		//do search stuff
		searchtype = 1 - searchtype;
		value += "Verse(s) containing \"";
		value += ref;
		value += "\": ";
		
		listkey = target->Search(ref, searchtype);
		
		if (strlen((const char*)listkey)) {
			if (!listkey.Error())
				value += (const char *)listkey;
			listkey++;
			while (!listkey.Error()) {
				value += " ; ";
				value += (const char *)listkey;
				listkey++;
			}
			value += " -- ";

			char *temp = new char[10];
			sprintf(temp, "%u", listkey.Count());
			value += temp;
			delete [] temp;

			value += " matches total (";
			value += target->Name();
			value += ")\n";
		}
		else {
			value += "none (";
			value += target->Name();
			value += ")\n";
		}
	}
	
	else if (querytype == QT_LD) {
		//do dictionary stuff
		
		target->SetKey(ref);
		
		char * text = (char *) *target;
		if (inputformat == FMT_GBF) {
				gbffilter = new GBFThML();
		        target->AddRenderFilter(gbffilter);
		}
		if (filter) target->AddRenderFilter(filter);
		if (outputformat == FMT_RTF && !strcmp(encoding.c_str(), "UTF-8")) {
				chfilter = new UnicodeRTF();
		        target->AddRenderFilter(chfilter);
		}
		else if ((outputformat == FMT_HTML || outputformat == FMT_THML) && strcmp(encoding.c_str(), "UTF-8")) {
		        chfilter = new Latin1UTF8();
				target->AddRenderFilter(chfilter);
		}

		if (outputformat == FMT_RTF) {
			value  = "{\\rtf1\\ansi{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f1\\fdecor\\fprq2 ";
			if (font)
				value += font;
			else
				value += "Times New Roman";
			value += ";}{\\f7\\froman\\fcharset2\\fprq2 Symbol;}}";
		}
		else if (outputformat == FMT_HTML) {
			value = "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">";
		}		
	
		if (strlen(text)) {
			value += (char*)target->KeyText();
			if (font && !filter) {
				value += ": <font face=\"";
				value += font;
				value += "\">";
			}
			else if (outputformat == FMT_RTF) {
				value += ": {\\f1 ";
			}
			else {
				value += ": ";
			}
			value += text;
			if (font && !filter) {
				value += "</font>";
			}
			else if (outputformat == FMT_RTF) {
				value += "}";
			}

			value += "(";
			value += target->Name();
			value += ")\n";
		}	

		if (outputformat == FMT_RTF) {
			value  += "}";
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
		
		if (inputformat == FMT_GBF) {
			gbffilter = new GBFThML();
			target->AddRenderFilter(gbffilter);
		}
		if (filter) target->AddRenderFilter(filter);
		if (outputformat == FMT_RTF && !strcmp(encoding.c_str(), "UTF-8")) {
				chfilter = new UnicodeRTF();
		        target->AddRenderFilter(chfilter);
		}
		else if ((outputformat == FMT_HTML || outputformat == FMT_THML) && strcmp(encoding.c_str(), "UTF-8")) {
				chfilter = new Latin1UTF8();
				target->AddRenderFilter(chfilter);
		}
		
 		listkey = vk.ParseVerseList(ref, "Gen1:1", true);
		int i;

		if (outputformat == FMT_RTF) {
			value  = "{\\rtf1\\ansi{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f1\\fdecor\\fprq2 ";
			if (font)
				value += font;
			else
				value += "Times New Roman";
			value += ";}{\\f7\\froman\\fcharset2\\fprq2 Symbol;}}";
		}
		else if (outputformat == FMT_HTML) {
			value = "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">";
		}

		for (i = 0; i < listkey.Count() && maxverses; i++) {
			VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.GetElement(i));
			if (element) {
				target->Key(element->LowerBound());
				vk = element->UpperBound();
				while (maxverses && target->Key() <= vk) {
					value += (char*)target->KeyText();
					if (font && !filter) {
						value += ": <font face=\"";
						value += font;
						value += "\">";
					}
					else if (outputformat == FMT_RTF) {
						value += ": {\\f1 ";
					}
					else {
						value += ": ";
					}
					value += (char const*)*target;
					if (font && !filter) {
						value += "</font>";
					}
					else if (outputformat == FMT_RTF) {
						value += "}";
					}

					if (inputformat != FMT_THML && !filter)
						value += "<br />";
					else if (outputformat == FMT_RTF)
						value += "\\par ";
					else if (outputformat == FMT_GBF)
						value += "<CM>";

					if (maxverses == 1) {
						value += " (";
						value += target->Name();
						value += ")";
					}
					value += "\n";
					maxverses--;
					(*target)++;
				}
			}
			else {
				target->Key(*listkey.GetElement(i));
				value += (char*)target->KeyText();
				if (font && !filter) {
					value += ": <font face=\"";
					value += font;
					value += "\">";
				}
				else if (outputformat == FMT_RTF) {
					value += ": {\\f1 ";
				}
				else {
					value += ": ";
				}
				value += (char const*)*target;
				if (font && !filter) {
					value += "</font>";
				}
				else if (outputformat == FMT_RTF) {
					value += "}";
				}
					
				if (inputformat != FMT_THML && !filter)
					value += "<br />";
				else if (outputformat == FMT_RTF)
					value += "\\par ";
				else if (outputformat == FMT_GBF)
					value += "<CM>";

				if (maxverses == 1) {
					value += " (";
					value += target->Name();
					value += ")";
				}
				value += "\n";
				maxverses--;
			}
		}

		if (outputformat == FMT_RTF) {
			value  += "}";
		}

	}
			
	char * versevalue = new char[value.length() + 1];
	strcpy (versevalue, value.c_str());

	if (filter) {
		target->RemoveRenderFilter(filter);
		delete filter;
	}
	if (chfilter) {
		target->RemoveRenderFilter(chfilter);
		delete chfilter;
	}
	if (gbffilter) {
		target->RemoveRenderFilter(gbffilter);
		delete gbffilter;
	}

	return versevalue;
}
