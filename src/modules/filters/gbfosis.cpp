/******************************************************************************
 *
 * gbfstrongs -	SWFilter decendant to hide or show strongs number
 *			in a GBF module.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <gbfosis.h>
#include <swmodule.h>
#include <versekey.h>
#include <swlog.h>
#include <stdarg.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

GBFOSIS::GBFOSIS() {
}


GBFOSIS::~GBFOSIS() {
}


char GBFOSIS::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module) {

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
	char *fromStart;
	bool newText = false;
	bool newWord = false;
	std::string tmp;
	bool suspendTextPassThru = false;
	bool keepToken = false;
	bool handled = false;

	len = strlen(text) + 1;	// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;
	
	textStart = from;
	fromStart = from;
	wordStart = text;

	static QuoteStack quoteStack;

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
				if (strchr(";, .:?!()'\"", *wordStart) && wordStart[0] && wordStart[1])
					wordStart++;
				else break;
			}
			while (wordEnd > wordStart) {
				if (strchr(" ,;.:?!()'\"", *wordEnd))
					wordEnd--;
				else break;
			}

			// Scripture Reference
			if (!strncmp(token, "scripRef", 8)) {
	//			pushString(buf, "<reference work=\"Bible.KJV\" reference=\"");
				suspendTextPassThru = true;
				newText = true;
				handled = true;
			}
			else	if (!strncmp(token, "/scripRef", 9)) {
				tmp = "";
				tmp.append(textStart, (int)(textEnd - textStart)+1);
				pushString(&to, convertToOSIS(tmp.c_str(), key));
				lastspace = false;
				suspendTextPassThru = false;
				handled = true;
			}

			// Footnote
			if (!strcmp(token, "RF")) {
	//			pushString(buf, "<reference work=\"Bible.KJV\" reference=\"");
				pushString(&to, "<note type=\"x-StudyNote\">");
				newText = true;
				lastspace = false;
				handled = true;
			}
			else	if (!strcmp(token, "Rf")) {
				pushString(&to, "</note>");
				lastspace = false;
				handled = true;
			}
			// hebrew titles
			if (!strcmp(token, "TH")) {
				pushString(&to, "<title type=\"psalm\">");
				newText = true;
				lastspace = false;
				handled = true;
			}
			else	if (!strcmp(token, "Th")) {
				pushString(&to, "</title>");
				lastspace = false;
				handled = true;
			}
			// Italics assume transchange
			if (!strcmp(token, "FI")) {
				pushString(&to, "<transChange type=\"added\">");
				newText = true;
				lastspace = false;
				handled = true;
			}
			else	if (!strcmp(token, "Fi")) {
				pushString(&to, "</transChange>");
				lastspace = false;
				handled = true;
			}
			// Paragraph break.  For now use empty paragraph element
			if (!strcmp(token, "CM")) {
				pushString(&to, "<milestone type=\"x-p\" />");
				newText = true;
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
				lastspace = false;
				handled = true;
			}

			// Strongs numbers
			else if (*token == 'W' && (token[1] == 'G' || token[1] == 'H')) {	// Strongs
				bool divineName = false;
				if (module->isProcessEntryAttributes()) {
					valto = val;
					for (unsigned int i = 1; ((token[i]) && (i < 150)); i++)
						*valto++ = token[i];
					*valto = 0;
					// normal strongs number
					strstrip(val);
					if (!strncmp(wordStart, "<w ", 3)) {
						strtok(wordStart, ">");
						char *attStart = strstr(wordStart, "lemma");
						if (attStart) {
							attStart += 7;
							sprintf(buf, "x-Strongs:%s|", val);
						}
						else {
							attStart = wordStart + 3;
							sprintf(buf, "lemma=\"x-Strongs:%s\" ", val);
						}
						wordStart[strlen(wordStart)] = '>';
						memmove(attStart+strlen(buf), attStart, (to-attStart)+1);
						memcpy(attStart, buf, strlen(buf));
						to+=strlen(buf);
					}
					else {
						if (!strcmp(val, "H03068")) {	//divineName
							sprintf(buf, "<divineName><w lemma=\"x-Strongs:%s\">", val);
							divineName = true;
						}
						else sprintf(buf, "<w lemma=\"x-Strongs:%s\">", val);

						memmove(wordStart+strlen(buf), wordStart, (to-wordStart)+1);
						memcpy(wordStart, buf, strlen(buf));
						to+=strlen(buf);

						if (divineName) {
							wordStart += 12;
							pushString(&to, "</w></divineName>");
						}
						else	pushString(&to, "</w>");

						module->getEntryAttributes()["Word"][wordstr]["Strongs"] = val;
						lastspace = false;
					}
				}
				handled = true;
			}

			// Morphology
			else if (*token == 'W' && token[1] == 'T' && (token[2] == 'G' || token[2] == 'H')) {	// Strongs
				valto = val;
				for (unsigned int i = 1; ((token[i]) && (i < 150)); i++)
					*valto++ = token[i];
				*valto = 0;
				strstrip(val);
				if (!strncmp(wordStart, "<w ", 3)) {
					strtok(wordStart, ">");
					char *attStart = strstr(wordStart, "morph");
					if (attStart) {
						attStart += 7;
						sprintf(buf, "x-%s:%s|", "StrongsMorph", val);
					}
					else {
						attStart = wordStart + 3;
						sprintf(buf, "morph=\"x-%s:%s\" ", "StrongsMorph", val);
					}
					wordStart[strlen(wordStart)] = '>';
					memmove(attStart+strlen(buf), attStart, (to-attStart)+1);
					memcpy(attStart, buf, strlen(buf));
					to+=strlen(buf);
				}
				else {
					sprintf(buf, "<w morph=\"x-%s:%s\">", "StrongsMorph", val);
					memmove(wordStart+strlen(buf), wordStart, (to-wordStart)+1);
					memcpy(wordStart, buf, strlen(buf));
					to+=strlen(buf);
					pushString(&to, "</w>");
					lastspace = false;
				}
				handled = true;
			}

			if (!keepToken) {	
				if (!handled) {
					SWLog::systemlog->LogError("Unprocessed Token: <%s>", token);
//					exit(-1);
				}
				if (from[1] && strchr(" ,;.:?!()'\"", from[1])) {
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
			switch (*from) {
			case '\'':
			case '\"':
			case '`':
//				quoteStack.handleQuote(fromStart, from, &to);
				*to++ = *from;
				from++;
				break;
			default:
				if (newWord && (*from != ' ')) {wordStart = to; newWord = false; memset(to, 0, 10); }
				if (!suspendTextPassThru) {
					*to++ = *from;
					lastspace = (*from == ' ');
				}
			}
		}
	}

	VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);
	if (vkey) {
		char ref[254];
		if (vkey->Verse())
			sprintf(ref, "\t\t<verse osisID=\"%s\">", vkey->getOSISRef());
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
						if (!quoteStack.empty()) {
							SWLog::systemlog->LogError("popping unclosed quote at end of book");
							quoteStack.clear();
						}
					}
				}
			}

			/*
			else if (vkey->Chapter()) {
				sprintf(ref, "\t<div type=\"chapter\" osisID=\"%s\">", vkey->getOSISRef());
			}
			else sprintf(ref, "\t<div type=\"book\" osisID=\"%s\">", vkey->getOSISRef());
			*/
		}
	}
	*to++ = 0;
	*to = 0;
	return 0;
}


void GBFOSIS::pushString(char **buf, const char *format, ...) {
  va_list argptr;

  va_start(argptr, format);
  *buf += vsprintf(*buf, format, argptr);
  va_end(argptr);

//  *buf += strlen(*buf);
}


const char *GBFOSIS::convertToOSIS(const char *inRef, const SWKey *key) {
	static std::string outRef;

	outRef = "";

	VerseKey defLanguage;
	ListKey verses = defLanguage.ParseVerseList(inRef, (*key), true);
	const char *startFrag = inRef;
	for (int i = 0; i < verses.Count(); i++) {
		VerseKey *element = SWDYNAMIC_CAST(VerseKey, verses.GetElement(i));
		char buf[5120];
		char frag[800];
		if (element) {
			memmove(frag, startFrag, ((const char *)element->userData - startFrag) + 1);
			frag[((const char *)element->userData - startFrag) + 1] = 0;
			startFrag = (const char *)element->userData + 1;
			sprintf(buf, "<reference osisRef=\"%s-\"%s\">%s</reference>", element->LowerBound().getOSISRef(), element->UpperBound().getOSISRef(), frag);
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


QuoteStack::QuoteStack() {
	clear();
}


void QuoteStack::clear() {
	while (!quotes.empty()) quotes.pop();
}


QuoteStack::~QuoteStack() {
	clear();
}


void QuoteStack::handleQuote(char *buf, char *quotePos, char **to) {
//QuoteInstance(char startChar = '\"', char level = 1, string uniqueID = "", char continueCount = 0) {
	if (!quotes.empty()) {
		QuoteInstance last = quotes.top();
		if (last.startChar == *quotePos) {
			GBFOSIS::pushString(to, "</quote>");
			quotes.pop();
		}
		else {
			quotes.push(QuoteInstance(*quotePos, last.level+1));
			quotes.top().pushStartStream(to);
		}
	}
	else {
		quotes.push(QuoteInstance(*quotePos));
		quotes.top().pushStartStream(to);
	}
}

void QuoteStack::QuoteInstance::pushStartStream(char **to) {
	GBFOSIS::pushString(to, "<quote level=\"%d\">", level);
}

SWORD_NAMESPACE_END
