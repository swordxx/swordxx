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
	
	SWModule *target;
	SWFilter * filter = 0;
	char *font = 0;
	char inputformat = 0;
	string value = "";
	char querytype = 0;	
	
	char * ref2 = new char[strlen(ref)];
	strcpy(ref2, ref);

	//deal with queries to "system"
	if (!stricmp(text, "system")) {
		querytype = QT_SYSTEM;
		return systemquery(ref2);
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
	if (optionfilters & OP_HEADINGS)
		manager.setGlobalOption("Headings","On");
	if (optionfilters & OP_STRONGS)
		manager.setGlobalOption("Strong's Numbers","On");
	if (optionfilters & OP_MORPH)
		manager.setGlobalOption("Morphological Tags","On");
	
	if (querytype == QT_SEARCH) {
		//do search stuff
		searchtype = 1 - searchtype;
		value += "Verse(s) containing \"";
		value += ref2;
		value += "\": ";
		
		listkey = target->Search(ref2, searchtype);
		
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
		
		target->SetKey(ref2);
		
		char * text = (char *) *target;
		if (inputformat == FMT_GBF) {
			target->AddRenderFilter(new GBFThML());
		}
		if (filter) target->AddRenderFilter(filter);
		if (strlen(text)) {
			value += target->KeyText();
			value += " : ";
			value += text;
			value += "\n(";
			value += target->Name();
			value += ")\n";
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
			target->AddRenderFilter(new GBFThML());
		}
		if (filter) target->AddRenderFilter(filter);
		
 		listkey = vk.ParseVerseList(ref2, "Gen1:1", true);
		int i;
		/*
		for (i = 0; i < listkey.Count() && maxverses; i++) {
			VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.GetElement(i));
			if (element) {
				printf("%s-%s", (const char*)element->LowerBound(), (const char*)element->UpperBound());
			}
			else {
				printf("%s", (const char*)*listkey.GetElement(i));
			}
		}
		*/
		for (i = 0; i < listkey.Count() && maxverses; i++) {
			VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.GetElement(i));
			if (element) {
				target->Key(element->LowerBound());
				vk = element->UpperBound();
				while (maxverses && target->Key() <= vk) {
					if (font && !filter) {
						value += (char*)target->KeyText();
						value += ": <font face=\"";
						value += font;
						value += "\">";
						value += (char const*)*target;
						value += "</font>";
					} else {
						value += (char*)target->KeyText();
						value += ": ";
						value += (char const*)*target;
						value += "";
					}
					if (inputformat != FMT_THML && !filter)
						value += "<br />";
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
				if (font && !filter) {
					value += (char*)target->KeyText();
					value += ": <font face=\"";
					value += font;
					value += "\">";
					value += (char const*)*target;
					value += "</font>";
				} else {
					value += (char*)target->KeyText();
					value += ": ";
					value += (char const*)*target;
					value += "";
				}
				if (inputformat != FMT_THML && !filter)
					value += "<br />";
				if (maxverses == 1) {
					value += " (";
					value += target->Name();
					value += ")";
				}
				value += "\n";
				maxverses--;
			}
		}
		/*
		for (i = 0;i < listkey.Count() && maxverses;i++) {
		  target->Key(*listkey.GetElement(i));
		  if (font && !filter) {
		    value += (char*)target->KeyText();
		    value += ": <font face=\"";
		    value += font;
		    value += "\">";
		    value += (char const*)*target;
		    value += "</font>";
		  } else {
		    value += (char*)target->KeyText();
		    value += ": ";
		    value += (char const*)*target;
		    value += "";
		  }
		  if (inputformat != FMT_THML && !filter)
		    value += "<br />";
		  if (i + 1 == maxverses) {
		    value += " (";
		    value += target->Name();
		    value += ")";
		  }
		  value += "\n";
		  maxverses--;
		}
		*/
	}
			
	delete filter;

	char * versevalue = new char[value.length() + 1];
	strcpy (versevalue, value.c_str());
	
	return versevalue;

}
