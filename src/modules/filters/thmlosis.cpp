/******************************************************************************
 *
 * thmlstrongs -	SWFilter decendant to hide or show strongs number
 *			in a ThML module.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <thmlosis.h>
#include <swmodule.h>
#include <swlog.h>
#include <versekey.h>
#include <stdarg.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

ThMLOSIS::ThMLOSIS() {
}


ThMLOSIS::~ThMLOSIS() {
}


char ThMLOSIS::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module) {

	char *to, *from, token[2048]; // cheese.  Fix.
	int tokpos = 0;
	bool intoken = false;
	int len;
	bool lastspace = false;
	int word = 1;
	char val[128];
	char buf[128];
	char wordstr[5];
	char *valto;
	char *ch;
	char *textStart, *textEnd;
	char *wordStart, *wordEnd;
	bool newText = false;
	bool newWord = false;
	std::string tmp;
	bool suspendTextPassThru = false;
	bool keepToken = false;
	bool handled = false;
	std::string divEnd = "";

	len = strlen(text) + 1;	// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;
	
	textStart = from;
	wordStart = text;

	// -------------------------------

	for (to = text; *from; from++) {
		if (*from == '<') {
			intoken = true;
			tokpos = 0;
			token[0] = 0;
			token[1] = 0;
			token[2] = 0;
			textEnd = from-1;
			wordEnd = to;
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;
			keepToken = false;
			suspendTextPassThru = false;
			newWord = true;
			handled = false;

			while (wordStart < (text+maxlen)) {
//				if (strchr(" ,;.?!()'\"", *wordStart))
				if (strchr(";,: .?!()'\"", *wordStart))
					wordStart++;
				else break;
			}
			while (wordEnd > wordStart) {
				if (strchr(" ,;:.?!()'\"", *wordEnd))
					wordEnd--;
				else break;
			}

			// section titles
			if (!strcmp(token, "div class=\"sechead\"")) {
				pushString(&to, "<title>");
				divEnd = "</title>";
				newText = true;
				lastspace = false;
				handled = true;
			}
			else	if (!strcmp(token, "/div")) {
				pushString(&to, divEnd.c_str());
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
				tmp = "";
				tmp.append(textStart, (int)(textEnd - textStart)+1);
				pushString(&to, convertToOSIS(tmp.c_str(), key));
				suspendTextPassThru = false;
				handled = true;
			}
/*      Usage of italics to represent transChange isn't domaninant;
        solution: mark in OSIS instead, assume no semantics other than emphasis
                of italicized text
                        if (!strcmp(module->Type(), "Biblical Texts")) {
        			// Italics assume transchange for Biblical texts
	        		if (!stricmp(token, "i")) {
		        		pushString(&to, "<transChange type=\"added\">");
			        	newText = true;
				        lastspace = false;
        				handled = true;
	        		}
		        	else	if (!stricmp(token, "/i")) {
			        	pushString(&to, "</transChange>");
        				lastspace = false;
	        			handled = true;
		        	}
                        }
                        else {
                        	// otherwise, italics are just italics
*/
	        		if (!stricmp(token, "i")) {
		        		pushString(&to, "<hi type=\"i\">");
			        	newText = true;
				        lastspace = false;
        				handled = true;
	        		}
		        	else	if (!stricmp(token, "/i")) {
			        	pushString(&to, "</hi>");
        				lastspace = false;
	        			handled = true;
		        	}
//                        }

	        	if (!strcmp(token, "b")) {
		        	pushString(&to, "<hi type=\"b\">");
			       	newText = true;
			        lastspace = false;
        			handled = true;
	        	}
		        else	if (!strcmp(token, "/b")) {
			       	pushString(&to, "</hi>");
        			lastspace = false;
	        		handled = true;
                        }

			// Footnote
			if (!strcmp(token, "note")) {
		        	pushString(&to, "<note>");
				newText = true;
                                lastspace = false;
				handled = true;
			}
			else	if (!strcmp(token, "/note")) {
				pushString(&to, "</note>");
                                lastspace = false;
				handled = true;
			}

			// Figure
			else	if (!strncmp(token, "img ", 4)) {
				const char *src = strstr(token, "src");
				if (!src)		// assert we have a src attribute
					continue;
//					return false;

				pushString(&to, "<figure src=\"");
				const char *c;
				for (c = src;((*c) && (*c != '"')); c++);

				/* uncomment for SWORD absolute path logic
				if (*(c+1) == '/') {
					pushString(buf, "file:");
					pushString(buf, module->getConfigEntry("AbsoluteDataPath"));
					if (*((*buf)-1) == '/')
						c++;		// skip '/'
				}
				end of uncomment for asolute path logic */

				for (c++;((*c) && (*c != '"')); c++)
					*to++ = *c;

				pushString(&to, "\" />");
				handled = true;
			}

			// Strongs numbers
			else	if (!strnicmp(token, "sync type=\"Strongs\" ", 20)) {	// Strongs
				if (module->isProcessEntryAttributes()) {
					valto = val;
					for (unsigned int i = 27; token[i] != '\"' && i < 150; i++)
						*valto++ = token[i];
					*valto = 0;
					if (atoi((!isdigit(*val))?val+1:val) < 5627) {
						// normal strongs number
						strstrip(val);
						sprintf(buf, "<w lemma=\"x-Strong:%s\">", val);
						memmove(wordStart+strlen(buf), wordStart, (to-wordStart)+1);
						memcpy(wordStart, buf, strlen(buf));
						to+=strlen(buf);
						pushString(&to, "</w>");
						module->getEntryAttributes()["Word"][wordstr]["Strongs"] = val;
//						tmp = "";
//						tmp.append(textStart, (int)(wordEnd - wordStart));
//						module->getEntryAttributes()["Word"][wordstr]["Text"] = tmp;
					}
					else {
						// verb morph
						sprintf(wordstr, "%03d", word-1);
						module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
					}
				}
				handled = true;
			}

			// Morphology
			else	if (!strncmp(token, "sync type=\"morph\"", 17)) {
				for (ch = token+17; *ch; ch++) {
					if (!strncmp(ch, "class=\"", 7)) {
						valto = val;
						for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
							*valto++ = ch[i];
						*valto = 0;
						sprintf(wordstr, "%03d", word-1);
						strstrip(val);
						module->getEntryAttributes()["Word"][wordstr]["MorphClass"] = val;
					}
					if (!strncmp(ch, "value=\"", 7)) {
						valto = val;
						for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
							*valto++ = ch[i];
						*valto = 0;
						sprintf(wordstr, "%03d", word-1);
						strstrip(val);
						module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
					}
				}
				if (!strncmp(wordStart, "<w ", 3)) {

					const char *cls = "Unknown", *morph;

					if (module->getEntryAttributes()["Word"][wordstr]["Morph"].size() > 0) {
						if (module->getEntryAttributes()["Word"][wordstr]["MorphClass"].size() > 0)
							cls = module->getEntryAttributes()["Word"][wordstr]["MorphClass"].c_str();
						morph = module->getEntryAttributes()["Word"][wordstr]["Morph"].c_str();
					
						sprintf(buf, "morph=\"x-%s:%s\" ", cls, morph);
						memmove(wordStart+3+strlen(buf), wordStart+3, (to-wordStart)+1);
						memcpy(wordStart+3, buf, strlen(buf));
						to+=strlen(buf);
					}
				}
				handled = true;
			}

			if (!keepToken) {	// if we don't want strongs
				if (!handled) {
					SWLog::systemlog->LogError("Unprocessed Token: <%s>", token);
//					exit(-1);
				}
				if (strchr(" ,:;.?!()'\"", from[1])) {
					if (lastspace)
						to--;
				}
				if (newText) {textStart = from+1; newText = false; }
//				if (newWord) {wordStart = to; newWord = false; }
				continue;
			}
			// if not a strongs token, keep token in text
			*to++ = '<';
			for (char *tok = token; *tok; tok++)
				*to++ = *tok;
			*to++ = '>';
			if (newText) {textStart = to; newWord = false; }
//			if (newWord) {wordStart = to; newWord = false; }
			continue;
		}
		if (intoken) {
			if ((tokpos < 2045) && ((*from != 10)&&(*from != 13))) {
				token[tokpos++] = *from;
				token[tokpos+2] = 0;
			}
		}
		else	{
			if (newWord && (*from != ' ')) {wordStart = to; newWord = false; memset(to, 0, 10); }
			if (!suspendTextPassThru) {
				*to++ = *from;
				lastspace = (*from == ' ');
			}
		}
	}

	VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);
	if (vkey) {
		char ref[254];
		if (vkey->Verse())
			sprintf(ref, "<verse osisID=\"%s\">", vkey->getOSISRef());
		else *ref = 0;
		if (*ref) {
			memmove(text+strlen(ref), text, maxlen-strlen(ref)-1);
			memcpy(text, ref, strlen(ref));
			to+=strlen(ref);
			if (vkey->Verse()) {
				VerseKey tmp;
				tmp = *vkey;
				tmp.AutoNormalize(0);
				tmp.Headings(1);
				sprintf(ref, "</verse>");
				pushString(&to, ref);
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
					}
				}
			}

//			else if (vkey->Chapter())
//				sprintf(ref, "\t<div type=\"chapter\" osisID=\"%s\">", vkey->getOSISRef());
//			else sprintf(ref, "\t<div type=\"book\" osisID=\"%s\">", vkey->getOSISRef());
		}
	}
	*to++ = 0;
	*to = 0;
	return 0;
}


void ThMLOSIS::pushString(char **buf, const char *format, ...) {
  va_list argptr;

  va_start(argptr, format);
  *buf += vsprintf(*buf, format, argptr);
  va_end(argptr);

//  *buf += strlen(*buf);
}


const char *ThMLOSIS::convertToOSIS(const char *inRef, const SWKey *key) {
	static std::string outRef;

	outRef = "";

	VerseKey defLanguage;
	ListKey verses = defLanguage.ParseVerseList(inRef, (*key), true);
	const char *startFrag = inRef;
	for (int i = 0; i < verses.Count(); i++) {
		VerseKey *element = SWDYNAMIC_CAST(VerseKey, verses.GetElement(i));
		char buf[5120];
		char frag[5120];
		if (element) {
			memmove(frag, startFrag, ((const char *)element->userData - startFrag) + 1);
			frag[((const char *)element->userData - startFrag) + 1] = 0;
			startFrag = (const char *)element->userData + 1;
			sprintf(buf, "<reference osisRef=\"%s-%s\">%s</reference>", element->LowerBound().getOSISRef(), element->UpperBound().getOSISRef(), frag);
		}
		else {
			memmove(frag, startFrag, ((const char *)verses.GetElement(i)->userData - startFrag) + 1);
			frag[((const char *)verses.GetElement(i)->userData - startFrag) + 1] = 0;
			startFrag = (const char *)verses.GetElement(i)->userData + 1;
			sprintf(buf, "<reference osisRef=\"%s\">%s</reference>", VerseKey(*verses.GetElement(i)).getOSISRef(), frag);
		}
		outRef+=buf;
	}
	return outRef.c_str();
}

SWORD_NAMESPACE_END
