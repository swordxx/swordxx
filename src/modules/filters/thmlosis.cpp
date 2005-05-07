/******************************************************************************
 *
 * thmlstrongs -	SWFilter descendant to hide or show strongs number
 *			in a ThML module.
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <thmlosis.h>
#include <swmodule.h>
#include <swlog.h>
#include <versekey.h>
#include <stdarg.h>
#include <utilstr.h>
#include <utilxml.h>


SWORD_NAMESPACE_START

ThMLOSIS::ThMLOSIS() {
}


ThMLOSIS::~ThMLOSIS() {
}


char ThMLOSIS::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	char token[2048]; // cheese.  Fix.
	int tokpos = 0;
	bool intoken = false;
	bool keepToken = false;

//	static QuoteStack quoteStack;
	
	bool lastspace = false;
	int word = 1;
	char val[128];
	SWBuf buf;
	char *valto;
	char *ch;
	
	const char *wordStart = text.c_str();
	const char *wordEnd;
	
	const char *textStart;
	const char *textEnd;
		
	bool suspendTextPassThru = false;
	bool handled = false;
	bool newText = false;
	bool newWord = false;
	
// 	SWBuf tmp;
	SWBuf divEnd = "";

	SWBuf orig = text;
	const char* from = orig.c_str();

	text = "";
	for (from = orig.c_str(); *from; ++from) {

		// handle silly <variant word> items in greek whnu, remove when module is fixed
		if ((*from == '<') && (*(from+1) < 0)) {
			text += "&lt;";
			continue;
		}

		if (*from == '<') { //start of new token detected
			intoken = true;
			tokpos = 0;
			token[0] = 0;
			token[1] = 0;
			token[2] = 0;
			textEnd = from-1;
			wordEnd = text.c_str() + text.length();//not good, instead of wordEnd = to!

// 			wordEnd = to;
			continue;
		}
		
		// handle silly <variant word> items in greek whnu, remove when module is fixed
		if ((*from == '>') && (*(from-1) < 0)) {
			text += "&gt;";
			continue;
		}

		if (*from == '>') {	// process tokens
			intoken = false;
			keepToken = false;
			suspendTextPassThru = false;
			newWord = true;
			handled = false;

			while (wordStart < (text.c_str() + text.length())) { //hack
				if (strchr(";,. :?!()'\"", *wordStart) && wordStart[0] && wordStart[1])
					wordStart++;
				else break;
			}
			while (wordEnd > wordStart) {
				if (strchr(" ,;:.?!()'\"", *wordEnd))
					wordEnd--;
				else break;
			}

			// variants
			if (!strncmp(token, "div type=\"variant\"", 18)) {
				XMLTag tag = token;
				text.append("<seg type=\"x-variant\"");
				SWBuf cls = "x-class:";
				cls += tag.getAttribute("class");
				if (cls.length()>8)
					text.appendFormatted(" subType=\"%s\"", cls.c_str());

				text += ">";
				divEnd = "</seg>";
				newText = true;
				lastspace = false;
				handled = true;
			}
			// section titles
			if (!strcmp(token, "div class=\"sechead\"")) {
// 				pushString(&to, "<title>");
				text.append("<title>");
				divEnd = "</title>";
				newText = true;
				lastspace = false;
				handled = true;
			}
			else	if (!strcmp(token, "/div")) {
				//pushString(&to, divEnd.c_str());
				text.append(divEnd);
				lastspace = false;
				handled = true;
			}
			// Scripture Reference
			if (!strncmp(token, "scripRef", 8)) {
	//			pushString(buf, "<reference osisRef=\"");
				suspendTextPassThru = true;
				newText = true;
				handled = true;
			}
			else	if (!strncmp(token, "/scripRef", 9)) {
				SWBuf tmp;
				tmp = "";
				tmp.append(textStart, (int)(textEnd - textStart)+1);
				//pushString(&to, convertToOSIS(tmp.c_str(), key));
				text.append(VerseKey::convertToOSIS(tmp.c_str(), key));
				suspendTextPassThru = false;
				handled = true;
			}
//      Usage of italics to represent transChange isn't domaninant;
//        solution: mark in OSIS instead, assume no semantics other than emphasis
//                of italicized text
//                        if (!strcmp(module->Type(), "Biblical Texts")) {
//        			// Italics assume transchange for Biblical texts
//	        		if (!stricmp(token, "i")) {
//		        		pushString(&to, "<transChange type=\"added\">");
//			        	newText = true;
//				        lastspace = false;
//        				handled = true;
//	        		}
//		        	else	if (!stricmp(token, "/i")) {
//			        	pushString(&to, "</transChange>");
//        				lastspace = false;
//	        			handled = true;
//		        	}
//                        }
//                        else {
//                        	// otherwise, italics are just italics
//-- end italics for transchange
	        		if (!stricmp(token, "i")) {
// 		        		pushString(&to, "<hi type=\"i\">");
					text.append("<hi type=\"i\">");
			        	newText = true;
				     lastspace = false;
        				handled = true;
	        		}
		        	else	if (!stricmp(token, "/i")) {
// 			        	pushString(&to, "</hi>");
					text.append("</hi>");
        				lastspace = false;
	        			handled = true;
		        	}
//                        }

	        	if (!strcmp(token, "b")) {
// 		        	pushString(&to, "<hi type=\"b\">");
				text.append("<hi type=\"b\">");
				newText = true;
				lastspace = false;
        			handled = true;
	        	}
			else if (!strcmp(token, "/b")) {
// 			     pushString(&to, "</hi>");
				text.append("</hi>");
        			lastspace = false;
	        		handled = true;
			}

			// Footnote
			if (!strcmp(token, "note")) {
		        	//pushString(&to, "<note>");
				text.append("<note>");
				newText = true;
				lastspace = false;
				handled = true;
			}
			else	if (!strcmp(token, "/note")) {
				// pushString(&to, "</note>");
				text.append("</note>");
				lastspace = false;
				handled = true;
			}

			// Figure
			else	if (!strncmp(token, "img ", 4)) {
				const char *src = strstr(token, "src");
				if (!src)		// assert we have a src attribute
					continue;
//					return false;

				//pushString(&to, "<figure src=\"");
				text.append("<figure src=\"");

				const char* end = strchr(src+2, '"'); //start search behind src="
				
				if (end) { //append the path
					text.append(src+2, end - (src+2));
				}
								
// 				const char *c;
// 				for (c = src;((*c) && (*c != '"')); c++);

// uncomment for SWORD absolute path logic
//				if (*(c+1) == '/') {
//					pushString(buf, "file:");
//					pushString(buf, module->getConfigEntry("AbsoluteDataPath"));
//					if (*((*buf)-1) == '/')
//						c++;		// skip '/'
//				}
//				end of uncomment for asolute path logic 

// 				for (c++;((*c) && (*c != '"')); c++)
// 					*to++ = *c;

				//pushString(&to, "\" />");
				text.append("\" />");
				handled = true;
			}

			// Strongs numbers
			else	if (!strnicmp(token, "sync type=\"Strongs\" ", 20)) {	// Strongs
				valto = val;
				for (unsigned int i = 27; token[i] != '\"' && i < 150; i++)
					*valto++ = token[i];
				*valto = 0;
				if (atoi((!isdigit(*val))?val+1:val) < 5627) {
					// normal strongs number
					strstrip(val);

					if (!strncmp(wordStart, "<w ", 3)) {
						const char *attStart = strstr(wordStart, "lemma");
						if (attStart) { //existing morph attribute, append this one to it
							attStart += 7;
							buf = "";
							buf.appendFormatted("strong:%s ", val);
						}
						else { // no lemma attribute
							attStart = wordStart + 3;
							buf = "";
							buf.appendFormatted(buf, "lemma=\"strong:%s\" ", val);
						}

						text.insert(attStart - text.c_str(), buf);
					}
					else { //wordStart doesn't point to an existing <w> attribute!
						buf = "";
						buf.appendFormatted("<w lemma=\"strong:%s\">", val);
						text.insert(wordStart - text.c_str(), buf);
						text += "</w>";
						lastspace = false;
					}
				}
				// OLB verb morph, leave it out of OSIS tag
				else {
				}
				handled = true;
			}

			// Morphology
			else	if (!strncmp(token, "sync type=\"morph\"", 17)) {
				SWBuf cls = "";
				SWBuf morph = "";
				for (ch = token+17; *ch; ch++) {
					if (!strncmp(ch, "class=\"", 7)) {
						valto = val;
						for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
							*valto++ = ch[i];
						*valto = 0;
						strstrip(val);
						cls = val;
					}
					if (!strncmp(ch, "value=\"", 7)) {
						valto = val;
						for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
							*valto++ = ch[i];
						*valto = 0;
						strstrip(val);
						morph = val;
					}
				}
				if (!strncmp(wordStart, "<w ", 3)) {
					const char *attStart = strstr(wordStart, "morph");
					if (attStart) { //existing morph attribute, append this one to it
						attStart += 7;
						buf = "";
						buf.appendFormatted("%s:%s ", ((cls.length())?cls.c_str():"robinson"), morph.c_str());
					}
					else { // no lemma attribute
						attStart = wordStart + 3;
						buf = "";
						buf.appendFormatted("morph=\"%s:%s\" ", ((cls.length())?cls.c_str():"robinson"), morph.c_str());
					}
					
					text.insert(attStart - text.c_str(), buf); //hack, we have to
				}
				else { //no existing <w> attribute fond
					buf = "";
					buf.appendFormatted("<w morph=\"%s:%s\">", ((cls.length())?cls.c_str():"robinson"), morph.c_str());
					text.insert(wordStart - text.c_str(), buf);
					text += "</w>";
					lastspace = false;

				}
				handled = true;
			}

			if (!keepToken) {
				if (!handled) {
					SWLog::getSystemLog()->logError("Unprocessed Token: <%s> in key %s", token, key ? (const char*)*key : "<unknown>");
//					exit(-1);
				}
				if (from[1] && strchr(" ,;.:?!()'\"", from[1])) {
					if (lastspace) {
						text--;
					}
				}
				if (newText) {
					textStart = from+1; 
					newText = false; 
				}
				continue;
			}
			
			// if not a strongs token, keep token in text
			text.appendFormatted("<%s>", token);
			
			if (newText) {
				textStart = text.c_str() + text.length(); 
				newWord = false; 
			}
			continue;
		}
		if (intoken) {
			if ((tokpos < 2045) && ((*from != 10)&&(*from != 13))) {
				token[tokpos++] = *from;
				token[tokpos+2] = 0;
			}
		}
		else	{
			switch (*from) {
			case '\'':
			case '\"':
			case '`':
//				quoteStack.handleQuote(fromStart, from, &to);
				text += *from;
				//from++; //this line removes chars after an apostrophe! Needs fixing.
				break;
			default:
				if (newWord && (*from != ' ')) {
					wordStart = text.c_str() + text.length();
					newWord = false;
					
					//fix this if required?
					//memset(to, 0, 10); 
					
				}
				
				if (!suspendTextPassThru) {
					text += (*from);
					lastspace = (*from == ' ');
				}
			}
		}
	}

	VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);
	if (vkey) {
		SWBuf ref = "";
		if (vkey->Verse()) {
			ref.appendFormatted("\t\t<verse osisID=\"%s\">", vkey->getOSISRef());
		}
			
		if (ref.length() > 0) {
			
			text = ref + text;
			
			if (vkey->Verse()) {
				VerseKey tmp;
				tmp = *vkey;
				tmp.AutoNormalize(0);
				tmp.Headings(1);
				
				text += "</verse>";
				
				tmp = MAXVERSE;
				if (*vkey == tmp) {
					tmp.Verse(0);
//					sprintf(ref, "\t</div>");
//					pushString(&to, ref);
					tmp = MAXCHAPTER;
					tmp = MAXVERSE;
					if (*vkey == tmp) {
						tmp.Chapter(0);
						tmp.Verse(0);
//						sprintf(ref, "\t</div>");
//						pushString(&to, ref);
/*
						if (!quoteStack.empty()) {
							SWLog::getSystemLog()->logError("popping unclosed quote at end of book");
							quoteStack.clear();
						}
*/
					}
				}
			}
//			else if (vkey->Chapter()) {
//				sprintf(ref, "\t<div type=\"chapter\" osisID=\"%s\">", vkey->getOSISRef());
//			}
//			else sprintf(ref, "\t<div type=\"book\" osisID=\"%s\">", vkey->getOSISRef());
		}
	}
	return 0;
}


SWORD_NAMESPACE_END
