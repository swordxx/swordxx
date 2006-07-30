/***************************************************************************
                     osishtmlhref.cpp  -  OSIS to HTML with hrefs filter
                             -------------------
    begin                : 2003-06-24
    copyright            : 2003 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation version 2 of the License.                *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <ctype.h>
#include <osishtmlhref.h>
#include <utilxml.h>
#include <utilstr.h>
#include <versekey.h>
#include <swmodule.h>
#include <url.h>
#include <stack>

SWORD_NAMESPACE_START

class OSISHTMLHREF::QuoteStack : public std::stack<const char*> {
};

OSISHTMLHREF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
	quoteStack = new QuoteStack();
	wordsOfChristStart = "<font color=\"red\"> ";
	wordsOfChristEnd   = "</font> ";
	if (module) {
		osisQToTick = ((!module->getConfigEntry("OSISqToTick")) || (strcmp(module->getConfigEntry("OSISqToTick"), "false")));
		version = module->Name();
	}
	else {
		osisQToTick = true;	// default
		version = "";
	}
}

OSISHTMLHREF::MyUserData::~MyUserData() {
	// Just in case the quotes are not well formed
	while (!quoteStack->empty()) {
		const char *tagData = quoteStack->top();
		quoteStack->pop();
		delete tagData;
	}
	delete quoteStack;
}

OSISHTMLHREF::OSISHTMLHREF() {
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);

//   commenting these out.  If someone is sure we shouldn't
//   convert these since we are outputing to a markup that
//   recognizes them, then please delete these lines
//   addEscapeStringSubstitute("amp",  "&");
//   addEscapeStringSubstitute("apos", "'");
//   addEscapeStringSubstitute("lt",   "<");
//   addEscapeStringSubstitute("gt",   ">");
//   addEscapeStringSubstitute("quot", "\"");

	setTokenCaseSensitive(true);
	
	addTokenSubstitute("lg",  "<br />");
	addTokenSubstitute("/lg", "<br />");
}

// though this might be slightly slower, possibly causing an extra bool check, this is a renderFilter
// so speed isn't the absolute highest priority, and this is a very minor possible hit
static inline void outText(const char *t, SWBuf &o, BasicFilterUserData *u) { if (!u->suspendTextPassThru) o += t; }
static inline void outText(char t, SWBuf &o, BasicFilterUserData *u) { if (!u->suspendTextPassThru) o += t; }

bool OSISHTMLHREF::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
		MyUserData *u = (MyUserData *)userData;
		XMLTag tag(token);
		
		// <w> tag
		if (!strcmp(tag.getName(), "w")) {
 
			// start <w> tag
			if ((!tag.isEmpty()) && (!tag.isEndTag())) {
				u->w = token;
			}

			// end or empty <w> tag
			else {
				bool endTag = tag.isEndTag();
				SWBuf lastText;
				//bool show = true;	// to handle unplaced article in kjv2003-- temporary till combined

				if (endTag) {
					tag = u->w.c_str();
					lastText = u->lastTextNode.c_str();
				}
				else lastText = "stuff";

				const char *attrib;
				const char *val;
				if (attrib = tag.getAttribute("xlit")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outText(" ", buf, u);
					outText(val, buf, u);
				}
				if (attrib = tag.getAttribute("gloss")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outText(" ", buf, u);
					outText(val, buf, u);
				}
				if (attrib = tag.getAttribute("lemma")) {
					int count = tag.getAttributePartCount("lemma", ' ');
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("lemma", i, ' ');
						if (i < 0) i = 0;	// to handle our -1 condition
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;
						SWBuf gh;
						if(*val == 'G')
							gh = "Greek";
						if(*val == 'H')
							gh = "Hebrew";
						const char *val2 = val;
						if ((strchr("GH", *val)) && (isdigit(val[1])))
							val2++;
						//if ((!strcmp(val2, "3588")) && (lastText.length() < 1))
						//	show = false;
						//else {
							if (!u->suspendTextPassThru) {
								buf.appendFormatted(" <small><em>&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=%s&value=%s\">%s</a>&gt;</em></small> ", 
										(gh.length()) ? gh.c_str() : "", 
										URL::encode(val2).c_str(), 
										val2);
							}
						//}
						
					} while (++i < count);
				}
				if (attrib = tag.getAttribute("morph")) { // && (show)) {
					SWBuf savelemma = tag.getAttribute("savlm");
					//if ((strstr(savelemma.c_str(), "3588")) && (lastText.length() < 1))
					//	show = false;
					//if (show) {
						int count = tag.getAttributePartCount("morph", ' ');
						int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
						do {
							attrib = tag.getAttribute("morph", i, ' ');
							if (i < 0) i = 0;	// to handle our -1 condition
							val = strchr(attrib, ':');
							val = (val) ? (val + 1) : attrib;
							const char *val2 = val;
							if ((*val == 'T') && (strchr("GH", val[1])) && (isdigit(val[2])))
								val2+=2;
							if (!u->suspendTextPassThru) {
								buf.appendFormatted(" <small><em>(<a href=\"passagestudy.jsp?action=showMorph&type=%s&value=%s\">%s</a>)</em></small> ", 
										URL::encode(tag.getAttribute("morph")).c_str(),
										URL::encode(val).c_str(), 
										val2);
							}
						} while (++i < count);
					//}
				}
				if (attrib = tag.getAttribute("POS")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outText(" ", buf, u);
					outText(val, buf, u);
				}

				/*if (endTag)
					buf += "}";*/
			}
		}

		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				SWBuf type = tag.getAttribute("type");
				bool strongsMarkup = (type == "x-strongsMarkup" || type == "strongsMarkup");	// the latter is deprecated
				if (strongsMarkup) {
					tag.setEmpty(false);	// handle bug in KJV2003 module where some note open tags were <note ... />
				}

				if (!tag.isEmpty()) {

					if (!strongsMarkup) {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
						SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
						VerseKey *vkey;
						// see if we have a VerseKey * or descendant
						SWTRY {
							vkey = SWDYNAMIC_CAST(VerseKey, u->key);
						}
						SWCATCH ( ... ) {	}
						if (vkey) {
							//printf("URL = %s\n",URL::encode(vkey->getText()).c_str());
							char ch = ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n');
							buf.appendFormatted("<a href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup>*%c</sup></small></a> ", 
								ch, 
								URL::encode(footnoteNumber.c_str()).c_str(), 
								URL::encode(u->version.c_str()).c_str(), 
								URL::encode(vkey->getText()).c_str(), 
								ch);
						}
					}
				}
				u->suspendTextPassThru = true;
			}
			if (tag.isEndTag()) {
				u->suspendTextPassThru = false;
			}
		}

		// <p> paragraph tag
		else if (!strcmp(tag.getName(), "p")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
				outText("<!P><br />", buf, u);
			}
			else if (tag.isEndTag()) {	// end tag
				outText("<!/P><br />", buf, u);
				userData->supressAdjacentWhitespace = true;
			}
			else {					// empty paragraph break marker
				outText("<!P><br />", buf, u);
				userData->supressAdjacentWhitespace = true;
			}
		}

		// <reference> tag
		else if (!strcmp(tag.getName(), "reference")) {			
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText("<a href=\"\">", buf, u);
			}
			else if (tag.isEndTag()) {
				outText("</a>", buf, u);
			}
		}

		// <l> poetry, etc
		else if (!strcmp(tag.getName(), "l")) {
			// end line marker
			if (tag.getAttribute("eID")) {
				outText("<br />", buf, u);
			}
			// <l/> without eID or sID
			// Note: this is improper osis. This should be <lb/>
			else if (tag.isEmpty() && !tag.getAttribute("sID")) {
				outText("<br />", buf, u);
			}
			// end of the line
			else if (tag.isEndTag()) {
				outText("<br />", buf, u);
			}
		}

		// <lb.../>
		else if (!strcmp(tag.getName(), "lb")) {
			outText("<br />", buf, u);
			userData->supressAdjacentWhitespace = true;
		}
		// <milestone type="line"/>
		// <milestone type="x-p"/>
		// <milestone type="cQuote" marker="x"/>
		else if ((!strcmp(tag.getName(), "milestone")) && (tag.getAttribute("type"))) {
			if(!strcmp(tag.getAttribute("type"), "line")) {
				outText("<br />", buf, u);
				userData->supressAdjacentWhitespace = true;
			}
			else if(!strcmp(tag.getAttribute("type"),"x-p"))  {
				if( tag.getAttribute("marker"))
					outText(tag.getAttribute("marker"), buf, u);
				else outText("<!p>", buf, u);
			}
			else if (!strcmp(tag.getAttribute("type"), "cQuote")) {
				const char *mark = tag.getAttribute("marker");
				const char *lev = tag.getAttribute("level");
				int level = (lev) ? atoi(lev) : 1;

				// first check to see if we've been given an explicit mark
				if (mark)
					outText(mark, buf, u);
				// finally, alternate " and ', if config says we should supply a mark
				else if (u->osisQToTick)
					outText((level % 2) ? '\"' : '\'', buf, u);
			}
		}

		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText("<b>", buf, u);
			}
			else if (tag.isEndTag()) {
				outText("</b><br />", buf, u);
			}
		}

		// <catchWord> & <rdg> tags (italicize)
		else if (!strcmp(tag.getName(), "rdg") || !strcmp(tag.getName(), "catchWord")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText("<i>", buf, u);
			}
			else if (tag.isEndTag()) {
				outText("</i>", buf, u);
			}
		}

		// divineName  
		else if (!strcmp(tag.getName(), "divineName")) {
			SWBuf type = tag.getAttribute("type");
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "x-yhwh") {
					u->inName = true;
					u->suspendTextPassThru = true;
				} else {
					u->inName = false;
				}
			} else if (tag.isEndTag()) {
				if(u->inName ) {
					char firstChar = *u->lastTextNode.c_str();
					const char *name = u->lastTextNode.c_str();
					++name;
					outText(firstChar, buf, u);
					outText("<font size=\"-1\">", buf, u);
					
					for(int i=0;i<strlen(name);i++)
						outText(toupper(name[i]), buf, u);
					outText("</font>", buf, u);
					u->inName = false;
					u->suspendTextPassThru = false;
				}
			} 
		}

		// <hi> text highlighting
		else if (!strcmp(tag.getName(), "hi")) {
			SWBuf type = tag.getAttribute("type");
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "b" || type == "x-b") {
					outText("<b>", buf, u);
					u->inBold = true;
				}
				else {	// all other types
					outText("<i>", buf, u);
					u->inBold = false;
				}
			}
			else if (tag.isEndTag()) {
				if(u->inBold) {
					outText("</b>", buf, u);
					u->inBold = false;
				}
				else outText("</i>", buf, u);
			}
		}

		// <q> quote
		// Rules for a quote element:
		// If the tag is empty with an sID or an eID then use whatever it specifies for quoting.
		//    Note: empty elements without sID or eID are ignored.
		// If the tag is <q> then use it's specifications and push it onto a stack for </q>
		// If the tag is </q> then use the pushed <q> for specification
		// If there is a marker attribute, possibly empty, this overrides osisQToTick.
		// If osisQToTick, then output the marker, using level to determine the type of mark.
		else if (!strcmp(tag.getName(), "q")) {
			SWBuf type = tag.getAttribute("type");
			SWBuf who = tag.getAttribute("who");
			const char *lev = tag.getAttribute("level");
			const char *mark = tag.getAttribute("marker");
			int level = (lev) ? atoi(lev) : 1;

			// open <q> or <q sID... />
			if ((!tag.isEmpty()) || (tag.getAttribute("sID"))) {
				// if <q> then remember it for the </q>
				if (!tag.isEmpty()) {
					char *tagData = 0;
					stdstr(&tagData, tag.toString());
					u->quoteStack->push(tagData);
				}

				// Do this first so quote marks are included as WoC
				if (who == "Jesus")
					outText(u->wordsOfChristStart, buf, u);

				// first check to see if we've been given an explicit mark
				if (mark)
					outText(mark, buf, u);
				//alternate " and '
				else if (u->osisQToTick)
					outText((level % 2) ? '\"' : '\'', buf, u);
			}
			// close </q> or <q eID... />
			else if ((tag.isEndTag()) || (tag.getAttribute("eID"))) {
				// if it is </q> then pop the stack for the attributes
				if (tag.isEndTag() && !u->quoteStack->empty()) {
					const char *tagData  = u->quoteStack->top();
					u->quoteStack->pop();
					XMLTag qTag(tagData);
					delete tagData;

					type  = qTag.getAttribute("type");
					who   = qTag.getAttribute("who");
					lev   = qTag.getAttribute("level");
					mark  = qTag.getAttribute("marker");
					level = (lev) ? atoi(lev) : 1;
				}

				// first check to see if we've been given an explicit mark
				if (mark)
					outText(mark, buf, u);
				// finally, alternate " and ', if config says we should supply a mark
				else if (u->osisQToTick)
					outText((level % 2) ? '\"' : '\'', buf, u);

				// Do this last so quote marks are included as WoC
				if (who == "Jesus")
					outText(u->wordsOfChristEnd, buf, u);
			}
		}

		// <transChange>
		else if (!strcmp(tag.getName(), "transChange")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				SWBuf type = tag.getAttribute("type");
				u->lastTransChange = type;

				// just do all transChange tags this way for now
				if ((type == "added") || (type == "supplied"))
					outText("<i>", buf, u);
				else if (type == "tenseChange")
					buf += "*";
			}
			else if (tag.isEndTag()) {
				SWBuf type = u->lastTransChange;
				if ((type == "added") || (type == "supplied"))
					outText("</i>", buf, u);
			}
			else {	// empty transChange marker?
			}
		}

		// image
		else if (!strcmp(tag.getName(), "figure")) {
			const char *src = tag.getAttribute("src");
			if (!src)		// assert we have a src attribute
				return false;

			SWBuf filepath;
			if (userData->module) {
				filepath = userData->module->getConfigEntry("AbsoluteDataPath");
				if ((filepath.size()) && (filepath[filepath.size()-1] != '/') && (src[0] != '/'))
					filepath += '/';
			}
			filepath += src;

// we do this because BibleCS looks for this EXACT format for an image tag
			outText("<image src=\"", buf, u);
			outText(filepath, buf, u);
			outText("\" />", buf, u);
		}

		else {
		      return false;  // we still didn't handle token
		}
	}
	return true;
}


SWORD_NAMESPACE_END
