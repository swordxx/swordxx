// Diatheke 4.0 by Chris Little <chrislit@crosswire.org>
// Copyright 1999, 2000, 2001 by Chris Little
// Licensed under GNU General Public License (GPL)
// see accompanying LICENSE file for license details
#include "corediatheke.h"

int hasalpha (char * string) {
	for (; *string != 0; string++)
		if (isalpha(*string))
			return 1;
		return 0;
}

char * systemquery(const char * key){
	ModMap::iterator it;
	SWMgr manager;
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

	delete [] key;
	char * versevalue = new char[value.length() + 1];
	strcpy (versevalue, value.c_str());
	return versevalue;
}

char* doquery(int maxverses = -1, char outputformat = FMT_PLAIN, char optionfilters = 0, char searchtype = ST_NONE, const char *text = 0, const char *locale = 0, const char *ref = 0) { 
	ModMap::iterator it;
	SWMgr manager;
	SWModule *target;
	ListKey listkey;
	SectionMap::iterator sit;
	ConfigEntMap::iterator eit;
	SWFilter * filter = 0;
	int length;
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
		
		char * comma = strchr(ref2, ',');
		char * dash = strchr(ref2, '-');
		
		if (comma) {
			// if we're looking up a sequence of
			// verses (must be in same chapter)
			char * vers1 = strchr(ref2, ':') + 1;
			
			char * vers2 = new char[strlen(vers1)];
			strcpy (vers2, vers1);
			
			char * chap = new char[strlen(ref2) + 8];
			strcpy (chap, ref2);
			
			char * vers3 = strchr(chap, ':') + 1;
			*vers3 = 0;
			char * vers4 = new char;
			
			vers4 = strtok(vers2, ",");
       
			while (vers4) {
				strcpy (vers3, vers4);
				
				dash = strchr(chap, '-');
				if (dash) { 			  // if we're looking up a range...
					*dash = 0;				//break string at the dash
					dash++;
					char * temp = new char[strlen(chap) + 8];
					
					length = strchr(chap, ':') - chap + 1;
					
					strncpy (temp, chap, length);
					*(temp + length) = 0;
					strcat (temp, dash);
					strcpy (dash, temp);
					delete [] temp;
				}
				else dash = chap;
				
				for(target->Key(chap); target->Key()<(VerseKey)dash && --maxverses > 0;(*target)++) {					
					if (font && !filter) {
						value += (char*)target->KeyText();
						value += ": <font face=\"";
						value += font;
						value += "\">";
						value += (char const*)*target;
						value += "</font>\n";
					} else {
						value += (char*)target->KeyText();
						value += ": ";
						value += (char const*)*target;
						value += "\n";
					}
					if (inputformat != FMT_THML && !filter)
						value += "<br />\n";					
				}				
				if (font && !filter) {
					value += (char*)target->KeyText();
					value += ": <font face=\"";
					value += font;
					value += "\">";
					value += (char const*)*target;
					value += "</font> (";
					value += target->Name();
					value += ")\n";
				} else {
					value += (char*)target->KeyText();
					value += ": ";
					value += (char const*)*target;
					value += " (";
					value += target->Name();
					value += ")\n";
				}
				if (inputformat != FMT_THML && !filter)
					value += "<br />\n";
				
				vers4 = strtok(0, ",");
			}
			delete vers4;
			delete [] chap;
			delete [] vers2;
		}
		else {
			if (dash) { 				// if we're looking up a range...
				*dash = 0;				  //break string at the dash
				dash++;
				char * temp = new char[strlen(ref2)];
				
				if (!strchr (dash, ':')) { /// if range supplies only second verse number (no book/chapter) assume same book/chapter
					length = strchr(ref2, ':') - ref2 + 1;
					strncpy (temp, ref2, length);
					*(temp + length) = 0;
					strcat (temp, dash);
					strcpy (dash, temp);
				}
				else if (!hasalpha (dash)){ /// if range supplies only second chapter/verse (no book--has no letters) assume same book
					strcpy (temp, ref2);
					length = 0;
					while (!isalpha(*temp)) {temp++; length++;}
					while (isalpha(*temp)) {temp++; length++;}
					while (!isdigit(*temp)) {temp++; length++;}
					strncpy (temp, ref2, length);
					*(temp + length) = 0;
					strcat (temp, dash);
					strcpy (dash, temp);
				}
				delete [] temp;
			}
			else dash = ref2;
			
			for(target->Key(ref2); target->Key()<(VerseKey)dash && --maxverses > 0;(*target)++) {				
				if (font && !filter) {
					value += (char*)target->KeyText();
					value += ": <font face=\"";
					value += font;
					value += "\">";
					value += (char const*)*target;
					value += "</font>\n";
				} else {
					value += (char*)target->KeyText();
					value += ": ";
					value += (char const*)*target;
					value += "\n";
				}
				if (inputformat != FMT_THML && !filter)
					value += "<br />\n";
			}
			
			
			if (font && !filter) {
				value += (char*)target->KeyText();
				value += ": <font face=\"";
				value += font;
				value += "\">";
				value += (char const*)*target;
				value += "</font> (";
				value += target->Name();
				value += ")\n";
			} else {
				value += (char*)target->KeyText();
				value += ": ";
				value += (char const*)*target;
				value += " (";
				value += target->Name();
				value += ")\n";
			}
			if (inputformat != FMT_THML && !filter)
				value += "<br />\n";
		}
	}
	
	delete filter;

	char * versevalue = new char[value.length() + 1];
	strcpy (versevalue, value.c_str());
	
	return versevalue;
}
