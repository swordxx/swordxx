/***************************************************************************
                     thmlosis.cpp  -  ThML to OSIS filter
                             -------------------
    begin                : 1999-10-27
    copyright            : 2001 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thmlosis.h>
#include <versekey.h>
#include <swmodule.h>


ThMLOSIS::ThMLOSIS() {

	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);

	setTokenCaseSensitive(true);

}

bool ThMLOSIS::handleToken(char **buf, const char *token, DualStringMap &userData) {

	if (!substituteToken(buf, token)) {

		if (!strncmp(token, "scripRef", 8)) {
//			pushString(buf, "<reference work=\"Bible.KJV\" reference=\"");
			userData["suspendTextPassThru"] = "true";
			return true;
		}
		if (!strncmp(token, "/scripRef", 9)) {
			pushString(buf,
				convertToOSIS(userData["lastTextNode"].c_str())
			);
//			pushString(buf, "\" />");
			userData["suspendTextPassThru"] = "false";
			return true;
		}
		if (!strncmp(token, "img ", 4)) {
			const char *src = strstr(token, "src");
			if (!src)		// assert we have a src attribute
				return false;

			pushString(buf, "<figure entity=\"");
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
				*(*buf)++ = *c;

			pushString(buf, "\" />");
			return true;
		}
//	addTokenSubstitute("/note", ") }");

//        addTokenSubstitute("br", "\\line ");
//        addTokenSubstitute("br /", "\\line ");
//        addTokenSubstitute("i", "{\\i1 ");
//        addTokenSubstitute("/i", "}");
//        addTokenSubstitute("b", "{\\b1 ");
//        addTokenSubstitute("/b", "}");
//        addTokenSubstitute("p", "\\par ");
//
        //we need uppercase forms for the moment to support a few early ThML modules that aren't XHTML compliant
//        addTokenSubstitute("BR", "\\line ");
//        addTokenSubstitute("I", "{\\i1 ");
//        addTokenSubstitute("/I", "}");
//        addTokenSubstitute("B", "{\\b1 ");
//        addTokenSubstitute("/B", "}");
//        addTokenSubstitute("P", "\\par ");

	// manually process if it wasn't a simple substitution
		if (!strncmp(token, "sync type=\"Strongs\" value=\"", 27)) {
/*                        if (token[27] == 'H' || token[27] == 'G' || token[27] == 'A') {
        			pushString(buf, " {\\fs15 <");
                                for (unsigned int i = 28; token[i] != '\"'; i++)
                		        *(*buf)++ = token[i];
        			pushString(buf, ">}");
                        }
                        else if (token[27] == 'T') {
                                pushString(buf, " {\\fs15 (");
        			for (unsigned int i = 28; token[i] != '\"'; i++)
        				*(*buf)++ = token[i];
        			pushString(buf, ")}");
                        }
		}
		else if (!strncmp(token, "sync type=\"morph\" ", 18)) {
			pushString(buf, " {\\fs15 (");
			for (const char *tok = token + 5; *tok; tok++) {
				if (!strncmp(tok, "value=\"", 7)) {
					tok += 7;
					for (;*tok != '\"'; tok++)
						*(*buf)++ = *tok;
					break;
				}
			}

			pushString(buf, ")}");
*/		}
		else if (!strncmp(token, "sync type=\"lemma\" value=\"", 25)) {
			pushString(buf, "{\\fs15 (");
			for (unsigned int i = 25; token[i] != '\"'; i++)
				*(*buf)++ = token[i];
			pushString(buf, ")}");
		}
                else if (!strncmp(token, "div", 3)) {
                        *(*buf)++ = '{';
                        if (!strncmp(token, "div class=\"title\"", 17)) {
                                pushString(buf, "\\par\\i1\\b1 ");
                                userData["sechead"] = "true";
                        }
                        else if (!strncmp(token, "div class=\"sechead\"", 19)) {
                                pushString(buf, "\\par\\i1\\b1 ");
                                userData["sechead"] = "true";
                        }
                }
                else if (!strncmp(token, "/div", 4)) {
                        *(*buf)++ = '}';
                        if (userData["sechead"] == "true") {
                                pushString(buf, "\\par ");
                                userData["sechead"] == "false";
                        }
                }
                else if (!strncmp(token, "note", 4)) {
                        pushString(buf, " {\\i1\\fs15 (");
                }

		else {
			return false;  // we still didn't handle token
		}
	}
	return true;
}


char ThMLOSIS::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module) {
	char retVal = SWBasicFilter::ProcessText(text, maxlen, key, module);
	VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);
	if (vkey) {
		char ref[254];
		if (vkey->Verse())
			sprintf(ref, "<verseStart VerseStart=\"%s\" />", vkey->getOSISRef());
		else if (vkey->Chapter())
			sprintf(ref, "<chapterStart ChapterStart=\"%s\" />", vkey->getOSISRef());
		else if (vkey->Book())
			sprintf(ref, "<bookStart BookStart=\"%s\" />", vkey->getOSISRef());
		else *ref = 0;
		if (*ref) {
			memmove(text+strlen(ref), text, maxlen-strlen(ref)-1);
			memcpy(text, ref, strlen(ref));
			if (vkey->Verse()) {
				VerseKey tmp;
				tmp = *vkey;
				tmp.AutoNormalize(0);
				tmp.Headings(1);
				sprintf(ref, "<verseEnd refVerseStart=\"%s\" />", vkey->getOSISRef());
				strcat(text, ref);
				tmp = MAXVERSE;
				if (*vkey == tmp) {
					tmp.Verse(0);
					sprintf(ref, "<chapterEnd refChapterStart=\"%s\" />", tmp.getOSISRef());
					strcat(text, ref);
					tmp = MAXCHAPTER;
					tmp = MAXVERSE;
					if (*vkey == tmp) {
						tmp.Chapter(0);
						tmp.Verse(0);
						sprintf(ref, "<bookEnd refBookStart=\"%s\" />", tmp.getOSISRef());
						strcat(text, ref);
					}
				}
			}

			else if (vkey->Chapter())
				sprintf(ref, "<chapterStart ChapterStart=\"%s\" />", vkey->getOSISRef());
			else sprintf(ref, "<bookStart BookStart=\"%s\" />", vkey->getOSISRef());
		}
	}
	return retVal;
}


const char *ThMLOSIS::convertToOSIS(const char *inRef) {
	static string outRef;

	outRef = "";

	VerseKey defLanguage;
	ListKey verses = defLanguage.ParseVerseList(inRef, (*key), true);
	const char *startFrag = inRef;
	for (int i = 0; i < verses.Count(); i++) {
		VerseKey *element = SWDYNAMIC_CAST(VerseKey, verses.GetElement(i));
		char buf[512];
		char frag[80];
		if (element) {
			memmove(frag, startFrag, ((const char *)element->userData - startFrag) + 1);
			frag[((const char *)element->userData - startFrag) + 1] = 0;
			startFrag = (const char *)element->userData + 1;
			sprintf(buf, "<reference work=\"Bible.KJV\" reference=\"%s\" referenceEnd=\"%s\">%s</reference>", element->LowerBound().getOSISRef(), element->UpperBound().getOSISRef(), frag);
		}
		else {
			memmove(frag, startFrag, ((const char *)verses.GetElement(i)->userData - startFrag) + 1);
			frag[((const char *)verses.GetElement(i)->userData - startFrag) + 1] = 0;
			startFrag = (const char *)verses.GetElement(i)->userData + 1;
			sprintf(buf, "<reference work=\"Bible.KJV\" reference=\"%s\">%s</reference>", VerseKey(*verses.GetElement(i)).getOSISRef(), frag);
		}
		outRef+=buf;
	}
	return outRef.c_str();
}

