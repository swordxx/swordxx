// Diatheke 4.0 by Chris Little <chrislit@crosswire.org>
// Copyright 1999, 2000, 2001 by CrossWire Bible Society http://www.crosswire.org
// Licensed under GNU General Public License (GPL)
// see accompanying LICENSE file for license details
#include "corediatheke.h"
#include <string>
#include <list>
#include <iostream.h>

void systemquery(const char * key, ostream* output){
	SWMgr manager;
	ModMap::iterator it;

	SWModule *target;
	
	bool types = false, descriptions = false, names = false;

	if (!stricmp(key, "localelist")) {		
		LocaleMgr lm = LocaleMgr::systemLocaleMgr;
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

void doquery(int maxverses = -1, char outputformat = FMT_PLAIN, char optionfilters = 0, char searchtype = ST_NONE, const char *text = 0, const char *locale = 0, const char *ref = 0, ostream* output = &cout) { 
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
	string encoding;
	char querytype = 0;	

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

	if (locale) {
		LocaleMgr::systemLocaleMgr.setDefaultLocaleName(locale);
		vk.setLocale(locale);
	}
	
	if (outputformat == FMT_PLAIN)
		filter = new ThMLPlain();
	else if (outputformat == FMT_HTML)
		filter = new ThMLHTML();
	else if (outputformat == FMT_GBF)
		filter = new ThMLGBF();
	else if (outputformat == FMT_RTF)
		filter = new ThMLRTF();
	else if (outputformat == FMT_OLB)
		filter = new ThMLOLB();
	
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
	
		if (strlen(text)) {
			*output << (char*)target->KeyText();
			if (font && !filter) {
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
			if (font && !filter) {
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
					if (font && !filter) {
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
					if (font && !filter) {
						*output << "</font>";
					}
					else if (outputformat == FMT_RTF) {
						*output << "}";
					}

					if (inputformat != FMT_THML && !filter)
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
				if (font && !filter) {
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
				if (font && !filter) {
					*output << "</font>";
				}
				else if (outputformat == FMT_RTF) {
					*output << "}";
				}
					
				if (inputformat != FMT_THML && !filter)
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
}
