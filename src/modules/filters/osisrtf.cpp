/***************************************************************************
                     osisrtf.cpp  -  OSIS to RTF filter
                             -------------------
    begin                : 2003-02-15
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
#include <osisrtf.h>
#include <utilxml.h>
#include <utilstr.h>
#include <versekey.h>
#include <swmodule.h>
#include <stringmgr.h>
#include <stack>

SWORD_NAMESPACE_START

namespace {
	class MyUserData : public BasicFilterUserData {
	public:
		bool osisQToTick;
		bool BiblicalText;
		bool inXRefNote;
		std::stack<const char*> quoteStack;
		SWBuf w;
		SWBuf version;
		MyUserData(const SWModule *module, const SWKey *key);
		~MyUserData();
	};


	MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
		inXRefNote    = false;
		BiblicalText  = false;
		if (module) {
			version = module->Name();
			BiblicalText = (!strcmp(module->Type(), "Biblical Texts"));
		}	
		osisQToTick = ((!module->getConfigEntry("OSISqToTick")) || (strcmp(module->getConfigEntry("OSISqToTick"), "false")));
	}


	MyUserData::~MyUserData() {
		// Just in case the quotes are not well formed
		while (!quoteStack.empty()) {
			const char *tagData = quoteStack.top();
			quoteStack.pop();
			delete tagData;
		}
	}

};

OSISRTF::OSISRTF() {
	setTokenStart("<");
	setTokenEnd(">");
  
	setEscapeStart("&");
	setEscapeEnd(";");
  
	setEscapeStringCaseSensitive(true);

	addEscapeStringSubstitute("amp", "&");
	addEscapeStringSubstitute("apos", "'");
	addEscapeStringSubstitute("lt", "<");
	addEscapeStringSubstitute("gt", ">");
	addEscapeStringSubstitute("quot", "\"");
	addTokenSubstitute("lg", "{\\par}");
	addTokenSubstitute("/lg", "{\\par}");

	setTokenCaseSensitive(true);
}


BasicFilterUserData *OSISRTF::createUserData(const SWModule *module, const SWKey *key) {
	return new MyUserData(module, key);
}


bool OSISRTF::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
		MyUserData *u = (MyUserData *)userData;
		XMLTag tag(token);

		// <w> tag
		if (!strcmp(tag.getName(), "w")) {

			// start <w> tag
			if ((!tag.isEmpty()) && (!tag.isEndTag())) {
				buf += "{";
				u->w = token;
			}

			// end or empty <w> tag
			else {
				bool endTag = tag.isEndTag();
				SWBuf lastText;
				bool show = true;	// to handle unplaced article in kjv2003-- temporary till combined

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
					buf.appendFormatted(" {\\fs15 <%s>}", val);
				}
				if (attrib = tag.getAttribute("gloss")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" {\\fs15 <%s>}", val);
				}
				if (attrib = tag.getAttribute("lemma")) {
					int count = tag.getAttributePartCount("lemma", ' ');
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("lemma", i, ' ');
						if (i < 0) i = 0;	// to handle our -1 condition
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;
						const char *val2 = val;
						if ((strchr("GH", *val)) && (isdigit(val[1])))
							val2++;
						if ((!strcmp(val2, "3588")) && (lastText.length() < 1))
							show = false;
						else	buf.appendFormatted(" {\\cf3 \\sub <%s>}", val2);
					} while (++i < count);
				}
				if ((attrib = tag.getAttribute("morph")) && (show)) {
					SWBuf savelemma = tag.getAttribute("savlm");
					if ((strstr(savelemma.c_str(), "3588")) && (lastText.length() < 1))
						show = false;
					if (show) {
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
							buf.appendFormatted(" {\\cf4 \\sub (%s)}", val2);
						} while (++i < count);
					}
				}
				if (attrib = tag.getAttribute("POS")) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" {\\fs15 <%s>}", val);
				}

				if (endTag)
					buf += "}";
			}
		}

		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				if (!tag.isEmpty()) {
					SWBuf type = tag.getAttribute("type");

					if (		   (type != "x-strongsMarkup")	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
							&& (type != "strongsMarkup")		// deprecated
							) {
						SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
						VerseKey *vkey;
						// see if we have a VerseKey * or descendant
						SWTRY {
							vkey = SWDYNAMIC_CAST(VerseKey, u->key);
						}
						SWCATCH ( ... ) {	}
						if (vkey) {
							char ch = ((!strcmp(type.c_str(), "crossReference")) || (!strcmp(type.c_str(), "x-cross-ref"))) ? 'x':'n';
							buf.appendFormatted("{\\super <a href=\"\">*%c%i.%s</a>} ", ch, vkey->Verse(), footnoteNumber.c_str());
							u->inXRefNote = (ch == 'x');
						}
					}
					u->suspendTextPassThru = true;
				}
			}
			if (tag.isEndTag()) {
				u->suspendTextPassThru = false;
				u->inXRefNote = false;
			}
		}

		// <p> paragraph tag
		else if (!strcmp(tag.getName(), "p")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
				buf += "{\\par}";
			}
			else if (tag.isEndTag()) {	// end tag
				buf += "{\\par}";
				userData->supressAdjacentWhitespace = true;
			}
			else {					// empty paragraph break marker
				buf += "{\\par\\par}";
				userData->supressAdjacentWhitespace = true;
			}
		}

		// <reference> tag
		else if (!strcmp(tag.getName(), "reference")) {
			if (!u->inXRefNote) {	// only show these if we're not in an xref note
				if ((!tag.isEndTag()) && (!tag.isEmpty())) {
					buf += "{<a href=\"\">";
				}
				else if (tag.isEndTag()) {
					buf += "</a>}";
				}
			}
		}

		// <l> poetry
		else if (!strcmp(tag.getName(), "l")) {
			// end line marker
			if (tag.getAttribute("eID")) {
				buf += "{\\par}";
			}
			// <l/> without eID or sID
			// Note: this is improper osis. This should be <lb/>
			else if (tag.isEmpty() && !tag.getAttribute("sID")) {
				buf += "{\\par}";
			}
			// end of the line
			else if (tag.isEndTag()) {
				buf += "{\\par}";
			}
		}

		// <milestone type="line"/> or <lb.../>
		else if ((!strcmp(tag.getName(), "lb")) || ((!strcmp(tag.getName(), "milestone")) && (tag.getAttribute("type")) && (!strcmp(tag.getAttribute("type"), "line")))) {
			buf += "{\\par}";
			userData->supressAdjacentWhitespace = true;
		}

		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "{\\par\\i1\\b1 ";
			}
			else if (tag.isEndTag()) {
				buf += "\\par}";
			}
		}

		// <catchWord> & <rdg> tags (italicize)
		else if (!strcmp(tag.getName(), "rdg") || !strcmp(tag.getName(), "catchWord")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf += "{\\i1 ";
			}
			else if (tag.isEndTag()) {
				buf += "}";
			}
		}

		// <hi>
		else if (!strcmp(tag.getName(), "hi")) {
			SWBuf type = tag.getAttribute("type");
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "b" || type == "x-b")
					buf += "{\\b1 ";
				else	// all other types
					buf += "{\\i1 ";
			}
			else if (tag.isEndTag()) {
				buf += "}";
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
					u->quoteStack.push(tagData);
				}

				// Do this first so quote marks are included as WoC
				if (who == "Jesus")
					buf += "\\cf6 ";

				// first check to see if we've been given an explicit mark
				if (mark)
					buf += mark;
				//alternate " and '
				else if (u->osisQToTick)
					buf += (level % 2) ? '\"' : '\'';
			}
			// close </q> or <q eID... />
			else if ((tag.isEndTag()) || (tag.getAttribute("eID"))) {
				// if it is </q> then pop the stack for the attributes
				if (tag.isEndTag() && !u->quoteStack.empty()) {
					const char *tagData  = u->quoteStack.top();
					u->quoteStack.pop();
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
					buf += mark;
				// finally, alternate " and ', if config says we should supply a mark
				else if (u->osisQToTick)
					buf += (level % 2) ? '\"' : '\'';

				// Do this last so quote marks are included as WoC
				if (who == "Jesus")
					buf += "\\cf0 ";
			}
		}


		// <milestone type="cQuote" marker="x"/>
		else if (!strcmp(tag.getName(), "milestone") && tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "cQuote")) {
			const char *mark = tag.getAttribute("marker");
			const char *lev = tag.getAttribute("level");
			int level = (lev) ? atoi(lev) : 1;

			// first check to see if we've been given an explicit mark
			if (mark)
				buf += mark;
			// finally, alternate " and ', if config says we should supply a mark
			else if (u->osisQToTick)
				buf += (level % 2) ? '\"' : '\'';
		}

		// <transChange>
		else if (!strcmp(tag.getName(), "transChange")) {
			SWBuf type = tag.getAttribute("type");

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {

// just do all transChange tags this way for now
//				if (type == "supplied")
					buf += "{\\i1 ";
			}
			else if (tag.isEndTag()) {
				buf += "}";
			}
		}

		// <divineName>
		else if (!strcmp(tag.getName(), "divineName")) {

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				u->suspendTextPassThru = true;
			}
			else if (tag.isEndTag()) {
				u->suspendTextPassThru = false;
				SWBuf lastText = u->lastTextNode.c_str();
				if (lastText.size()) {
					toupperstr(lastText);
					buf.appendFormatted("{\\fs19%c\\fs16%s}", lastText[0], lastText.c_str()+1);
				}
			}
		}

		// <div>
		else if (!strcmp(tag.getName(), "div")) {

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				buf.append("\\par\\par\\pard ");
			}
			else if (tag.isEndTag()) {
			}
		}

		// image
		else if (!strcmp(tag.getName(), "figure")) {
			const char *src = tag.getAttribute("src");
			if (!src)		// assert we have a src attribute
				return false;

			char* filepath = new char[strlen(u->module->getConfigEntry("AbsoluteDataPath")) + strlen(token)];
			*filepath = 0;
			strcpy(filepath, userData->module->getConfigEntry("AbsoluteDataPath"));
			strcat(filepath, src);

// we do this because BibleCS looks for this EXACT format for an image tag
			buf+="<img src=\"";
			buf+=filepath;
			buf+="\" />";
/*
			char imgc;
			for (c = filepath + strlen(filepath); c > filepath && *c != '.'; c--);
			c++;
			FILE* imgfile;
				    if (stricmp(c, "jpg") || stricmp(c, "jpeg")) {
						  imgfile = fopen(filepath, "r");
						  if (imgfile != NULL) {
								buf += "{\\nonshppict {\\pict\\jpegblip ";
								while (feof(imgfile) != EOF) {
									   buf.appendFormatted("%2x", fgetc(imgfile));
								}
								fclose(imgfile);
								buf += "}}";
						  }
				    }
				    else if (stricmp(c, "png")) {
						  buf += "{\\*\\shppict {\\pict\\pngblip ";

						  buf += "}}";
				    }
*/
			delete [] filepath;
		}
		else {
			return false;  // we still didn't handle token
		}
	}
	return true;
}


SWORD_NAMESPACE_END

