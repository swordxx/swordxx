/******************************************************************************
 *
 *  corediatheke.cpp -	
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

/******************************************************************************
 * Diatheke by Chris Little <chrislit@crosswire.org>
 * http://www.crosswire.org/sword/diatheke
 */

#include "corediatheke.h"
#include <regex.h>
#include <list>
#include <utilstr.h>
#include <versekey.h>
#include <swmodule.h>

using std::list;
using std::cout;
using std::endl;
using std::ostream;

void systemquery(const char * key, ostream* output){
	DiathekeMgr manager;
	ModMap::iterator it;

	SWModule *target;
	
	bool types = false, descriptions = false, names = false;

	if (!::stricmp(key, "localelist")) {		
		LocaleMgr *lm = LocaleMgr::getSystemLocaleMgr();
		list<SWBuf> loclist =	lm->getAvailableLocales();
		list<SWBuf>::iterator li = loclist.begin();
		for (;li != loclist.end(); li++) {
		  *output << li->c_str() << endl;
		}
	}
	else if (!::stricmp(key, "modulelist")) {
		types = true;
		descriptions = true;
		names = true;
	}
	else if (!::stricmp(key, "modulelistnames")) {
		names = true;
	}
	else if (!::stricmp(key, "modulelistdescriptions")) {
		descriptions = true;
	}
	
	
	if (types || descriptions || names) {
		if (types) *output << "Biblical Texts:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->getType(), "Biblical Texts")) {
				if (names) *output << target->getName();
				if (names && descriptions) *output << " : ";
				if (descriptions) *output << target->getDescription();
				*output << endl;
			}
		}
		if (types) *output << "Commentaries:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->getType(), "Commentaries")) {
				if (names) *output << target->getName();
				if (names && descriptions) *output << " : ";
				if (descriptions) *output << target->getDescription();
				*output << endl;
			}
		}
		if (types) *output << "Dictionaries:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->getType(), "Lexicons / Dictionaries")) {
				if (names) *output << target->getName();
				if (names && descriptions) *output << " : ";
				if (descriptions) *output << target->getDescription();
				*output << endl;
			}
		}
		if (types) *output << "Generic books:\n";
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			target = it->second;
			if (!strcmp(target->getType(), "Generic Books")) {
				if (names) *output << target->getName();
				if (names && descriptions) *output << " : ";
				if (descriptions) *output << target->getDescription();
				*output << endl;
			}
		}

	}
}

void doquery(unsigned long maxverses = -1, unsigned char outputformat = FMT_PLAIN, unsigned char outputencoding = ENC_UTF8, unsigned long optionfilters = 0, unsigned char searchtype = ST_NONE, const char *range = 0, const char *text = 0, const char *locale = 0, const char *ref = 0, ostream* output = &cout, const char *script = 0, signed char variants = 0) {
	static DiathekeMgr manager(NULL, NULL, false, outputencoding, outputformat,
		((OP_BIDI & optionfilters) == OP_BIDI),
		((OP_ARSHAPE & optionfilters) == OP_ARSHAPE));

	ModMap::iterator it;
	ListKey listkey;
	SectionMap::iterator sit;
	ConfigEntMap::iterator eit;

	SWModule *target;
	char *font = 0;
	char inputformat = 0;
	SWBuf encoding;
	char querytype = 0;

	if (locale) {
		LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(locale);
	}

	//deal with queries to "system"
	if (!::stricmp(text, "system")) {
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
	SWKey *p = target->createKey();
        VerseKey *parser = SWDYNAMIC_CAST(VerseKey, p);
	if (!parser) {
        	delete p;
	        parser = new VerseKey();
	}

	if ((sit = manager.config->Sections.find((*it).second->getName())) != manager.config->Sections.end()) {
		if ((eit = (*sit).second.find("SourceType")) != (*sit).second.end()) {
			if (!::stricmp((char *)(*eit).second.c_str(), "GBF"))
				inputformat = FMT_GBF;
			else if (!::stricmp((char *)(*eit).second.c_str(), "ThML"))
				inputformat = FMT_THML;
			else if (!::stricmp((char *)(*eit).second.c_str(), "OSIS"))
				inputformat = FMT_OSIS;
			else if (!::stricmp((char *)(*eit).second.c_str(), "TEI"))
				inputformat = FMT_TEI;
		}
		encoding = ((eit = (*sit).second.find("Encoding")) != (*sit).second.end()) ? (*eit).second : (SWBuf)"";
	}


	if (querytype == QT_INFO) {
	  switch (inputformat) {
	  case FMT_THML :
	    *output << "ThML";
	    break;
	  case FMT_GBF :
	    *output << "GBF";
	    break;
	  case FMT_OSIS :
	    *output << "OSIS";
	    break;
	  case FMT_TEI :
	    *output << "TEI";
	    break;
	  default:
	    *output << "Other";
	  }
	  *output << ";";
	  *output << target->getType();
	  *output << ";";
	  delete parser;
	  return;
	}

	if (searchtype)
		querytype = QT_SEARCH;
	else if (!strcmp(target->getType(), "Biblical Texts"))
		querytype = QT_BIBLE;
	else if (!strcmp(target->getType(), "Commentaries"))
		querytype = QT_COMM;
	else if (!strcmp(target->getType(), "Lexicons / Dictionaries"))
		querytype = QT_LD;
	else if (!strcmp(target->getType(), "Generic Books"))
		querytype = QT_LD;

	manager.setGlobalOption("Footnotes", (optionfilters & OP_FOOTNOTES) ? "On": "Off");
	manager.setGlobalOption("Headings", (optionfilters & OP_HEADINGS) ? "On": "Off");
	manager.setGlobalOption("Strong's Numbers", (optionfilters & OP_STRONGS) ? "On": "Off");
	manager.setGlobalOption("Morphological Tags", (optionfilters & OP_MORPH) ? "On": "Off");
	manager.setGlobalOption("Hebrew Cantillation", (optionfilters & OP_CANTILLATION) ? "On": "Off");
	manager.setGlobalOption("Hebrew Vowel Points", (optionfilters & OP_HEBREWPOINTS) ? "On": "Off");
	manager.setGlobalOption("Greek Accents", (optionfilters & OP_GREEKACCENTS) ? "On": "Off");
	manager.setGlobalOption("Lemmas", (optionfilters & OP_LEMMAS) ? "On": "Off");
	manager.setGlobalOption("Cross-references", (optionfilters & OP_SCRIPREF) ? "On": "Off");
	manager.setGlobalOption("Words of Christ in Red", (optionfilters & OP_REDLETTERWORDS) ? "On": "Off");
	manager.setGlobalOption("Arabic Vowel Points", (optionfilters & OP_ARABICPOINTS) ? "On": "Off");
	manager.setGlobalOption("Glosses", (optionfilters & OP_GLOSSES) ? "On": "Off");
	manager.setGlobalOption("Transliterated Forms", (optionfilters & OP_XLIT) ? "On": "Off");
	manager.setGlobalOption("Enumerations", (optionfilters & OP_ENUM) ? "On": "Off");

	manager.setGlobalOption("Transliteration", (optionfilters & OP_TRANSLITERATOR && script) ? script : "Off");
	

	if ((optionfilters & OP_VARIANTS) && variants) {
		if (variants == -1)
			manager.setGlobalOption("Textual Variants", "All Readings");
		else if (variants == 1)
			manager.setGlobalOption("Textual Variants", "Secondary Reading");
	}
	else
		manager.setGlobalOption("Textual Variants", "Primary Reading");


	if (querytype == QT_SEARCH) {

	        //this test is just to determine if we've got SWKeys or VerseKeys
	        if (!strcmp(target->getType(), "Biblical Texts"))
		  querytype = QT_BIBLE;
		else if (!strcmp(target->getType(), "Commentaries"))
		  querytype = QT_BIBLE;
		else if (!strcmp(target->getType(), "Lexicons / Dictionaries"))
		  querytype = QT_LD;
		else if (!strcmp(target->getType(), "Generic Books"))
		  querytype = QT_LD;

		//do search stuff
		char st = 1 - searchtype;
		if (querytype == QT_BIBLE) {
		  *output << "Verses containing \"";
		}
		else *output << "Entries containing \"";
	        *output << ref;
		*output << "\"-- ";

 		if (range) {
 			ListKey scope = parser->parseVerseList(range, "Gen 1:1", true);
 			listkey = target->search(ref, st, REG_ICASE, &scope);
 		}
 		else listkey = target->search(ref, st, REG_ICASE);

		if (strlen((const char*)listkey)) {
		  if (!listkey.popError()) {
		    if (outputformat == FMT_CGI) *output << "<entry>";
		    if (querytype == QT_BIBLE) {
		      *parser = listkey;
		      *output << (const char *)*parser;
		    }
		    else *output << (const char *)listkey;
		    if (outputformat == FMT_CGI) *output << "</entry>";
		  }
		  listkey++;
		  while (!listkey.popError()) {
		    *output << " ; ";
		    if (outputformat == FMT_CGI) *output << "<entry>";
		    if (querytype == QT_BIBLE) {
		      *parser = listkey;
		      *output << (const char *)*parser;
		    }
		    else *output << (const char *)listkey;
		    if (outputformat == FMT_CGI) *output << "</entry>";
		    listkey++;
		  }
		  *output << " -- ";

		  char *temp = new char[10];
		  sprintf(temp, "%u", listkey.getCount());
		  *output << temp;
		  delete [] temp;

		  *output << " matches total (";
		  *output << target->getName();
		  *output << ")\n";
		}
		else {
		  *output << "none (";
		  *output << target->getName();
		  *output << ")\n";
		}
	}

	else if (querytype == QT_LD) {
		//do dictionary stuff

		target->setKey(ref);

		SWBuf text = target->renderText();

		if (outputformat == FMT_RTF) {
			*output << "{\\rtf1\\ansi{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f1\\fdecor\\fprq2 ";
			if (font)
				*output << font;
			else
				*output << "Times New Roman";
			*output << ";}}";
		}
		else if (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML) {
			*output << "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">";
		}
		else if (outputformat == FMT_LATEX) {
			*output << "\\documentclass[12pt]{article}\n";
			*output << "\\usepackage{fontspec}\n";
			*output << "\\usepackage{geometry}\n";
			*output << "\\usepackage{setspace}\n";
			*output << "\\usepackage{polyglossia}\n";
			if (font) {
				*output << "\\setmainfont{";
				*output << font;
				*output << "}";
			} 
			
			*output << "\\begin{document}\n";
			*output << "\\setlength{\\parskip}{3pt} \% 1ex plus 0.5ex minus 0.2ex}\n";
		}


		if (text.length()) {
			if (outputformat == FMT_LATEX) {
				*output << "\\paragraph\n";
			}
			
			*output << (char*)target->getKeyText();
			if (font && (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML || outputformat == FMT_THML || outputformat == FMT_CGI)) {
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
			if (font && (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML || outputformat == FMT_THML || outputformat == FMT_CGI)) {
				*output << "</font>";
			}
			else if (outputformat == FMT_RTF) {
				*output << "}";
			}

			*output << "(";
			*output << target->getName();
			*output << ")\n";
		}

		if (outputformat == FMT_RTF) {
			*output << "}";
		}
		else if (outputformat == FMT_LATEX) {
			*output << "\\end{document}";
		}

	}

	else if (querytype == QT_BIBLE || querytype == QT_COMM) {
		//do commentary/Bible stuff

		if ((sit = manager.config->Sections.find((*it).second->getName())) != manager.config->Sections.end()) {
			if ((eit = (*sit).second.find("Font")) != (*sit).second.end()) {
				font = (char *)(*eit).second.c_str();
				if (strlen(font) == 0) font = 0;
			}
		}

 		listkey = parser->parseVerseList(ref, "Gen1:1", true);
		int i;
		
		if (outputformat == FMT_RTF) {
			*output << "{\\rtf1\\ansi{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f1\\fdecor\\fprq2 ";
			if (font)
				*output << font;
			else
				*output << "Times New Roman";
			*output << ";}{\\f7\\froman\\fcharset2\\fprq2 Symbol;}}";
		}

		else if (outputformat == FMT_LATEX) {
			*output << "\\documentclass{article}\n";
			*output << "\\usepackage{geometry}\n";
			*output << "\\usepackage{setspace}\n";
			*output << "\\usepackage{polyglossia}\n";
			*output << "\\usepackage{lettrine}\n";
			*output << "\\usepackage[perpage,para]{footmisc}\n";
			*output << "\\alph{footnote}\n";
			
			if (font) {
				*output << "\\setmainfont{";
				*output << font;
				*output << "}";
			} 
			*output << "\\begin{document}\n";
			*output << "\\setlength{\\parskip}{3pt} \% 1ex plus 0.5ex minus 0.2ex}\n";
		}


		else if (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML) {
			*output << "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">";
		}

		for (i = 0; i < listkey.getCount() && maxverses; i++) {
			VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.getElement(i));
			if (element && element->isBoundSet()) {
			  target->setKey(element->getLowerBound());
				*parser = element->getUpperBound();
				while (maxverses && *target->getKey() <= *parser) {

					if (outputformat == FMT_LATEX) {
						VerseKey *outkey = new VerseKey(target->getKey());
						if (outkey->getVerse() == 1) {
							*output << "\\lettrine[lines=3]{";
							*output << outkey->getChapter();
							*output << "}{ }";
						}
						else {	
							*output << "\\textsuperscript{\\tiny{";
							*output << outkey->getVerse();
							*output << "}} ";
						}
					}	
					else { 						
						*output << (char*)target->getKeyText();
					}
					if (font && (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML || outputformat == FMT_THML || outputformat == FMT_CGI)) {
						*output << ": <font face=\"";
						*output << font;
						*output << "\">";
					}
					else if (outputformat == FMT_RTF) {
						*output << ": {\\f1 ";
					}
					else if (outputformat == FMT_LATEX) {
						*output << " ";
					}
					else {
						*output << ": ";
					}
						
					*output << target->renderText();
					
					
					if (font && (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML || outputformat == FMT_THML || outputformat == FMT_CGI)) {
						*output << "</font>";
					}
					else if (outputformat == FMT_RTF) {
						*output << "}";
					}

					if (inputformat != FMT_THML && (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML || outputformat == FMT_THML || outputformat == FMT_CGI))
						*output << "<br />";
					else if (outputformat == FMT_OSIS)
						*output << "<milestone type=\"line\"/>";
					else if (outputformat == FMT_RTF)
						*output << "\\par ";
					else if (outputformat == FMT_GBF)
						*output << "<CM>";

					*output << "\n";

					if (*target->getKey() == *parser)
					  break;
					maxverses--;
					(*target)++;
				}
			}
			else {
				target->setKey(*listkey.getElement(i));
	
				if (outputformat == FMT_LATEX) {
					VerseKey *outkey = new VerseKey(target->getKey());
					if (outkey->getVerse() == 1) {
						*output << "\\lettrine[lines=3]{";
						*output << outkey->getChapter();
						*output << "}{ }";
					}
					else {	
						*output << "\\textsuperscript{\\tiny{";
						*output << outkey->getVerse();
						*output << "}} ";
					}
				}	
				else { 						
					*output << (char*)target->getKeyText();
					}

				if (font && (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML || outputformat == FMT_THML || outputformat == FMT_CGI)) {
					*output << ": <font face=\"";
					*output << font;
					*output << "\">";
				}
				else if (outputformat == FMT_RTF) {
					*output << ": {\\f1 ";
				}
				else if (outputformat == FMT_LATEX) {
					*output << " ";
				}
				else {
					*output << ": ";
				}
				*output << target->renderText();
				if (font && (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML || outputformat == FMT_THML || outputformat == FMT_CGI)) {
					*output << "</font>";
				}
				else if (outputformat == FMT_RTF) {
					*output << "}";
				}

				if (inputformat != FMT_THML && (outputformat == FMT_HTML || outputformat == FMT_HTMLHREF || outputformat == FMT_XHTML || outputformat == FMT_THML || outputformat == FMT_CGI))
					*output << "<br />";
				else if (outputformat == FMT_OSIS)
					*output << "<milestone type=\"line\"/>";
				else if (outputformat == FMT_RTF)
					*output << "\\par ";
				else if (outputformat == FMT_LATEX)
					*output << "\n\\paragraph ";
				else if (outputformat == FMT_GBF)
					*output << "<CM>";

				*output << "\n";
				maxverses--;
			}
		}

		*output << "(";
		*output << target->getName();
		*output << ")\n";

		if (outputformat == FMT_RTF) {
			*output << "}";
		}
		else if (outputformat == FMT_LATEX) {
			*output << "\\end{document}\n";
		}
	}
	delete parser;
}

