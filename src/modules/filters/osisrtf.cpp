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
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <osisrtf.h>

SWORD_NAMESPACE_START

OSISRTF::OSISRTF()
{
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
  
  setTokenCaseSensitive(true);  
}

bool OSISRTF::handleToken(SWBuf &buf, const char *token, DualStringMap &userData) {
  // manually process if it wasn't a simple substitution
  if (!substituteToken(buf, token)) {
    //w
    if (!strncmp(token, "w", 1)) {
      buf += "{";
      userData["w"] == token;
    }
    else if (!strncmp(token, "/w", 2)) {

      pos1 = userData["w"].find("xlit=\"", 0);
      if (pos1 != string::npos) {
	pos1 = userData["w"].find(":", pos1) + 1;
	pos2 = userData["w"].find("\"", pos1) - 1;
	tagData = userData["w"].substr(pos1, pos2-pos1);
  buf.appendFormatted(" {\\fs15 <%s>}", tagData.c_str() );
      }
      pos1 = userData["w"].find("gloss=\"", 0);
      if (pos1 != string::npos) {
	pos1 = userData["w"].find(":", pos1) + 1;
	pos2 = userData["w"].find("\"", pos1) - 1;
	tagData = userData["w"].substr(pos1, pos2-pos1);
  buf.appendFormatted(" {\\fs15 <%s>}", tagData.c_str() );
      }
      pos1 = userData["w"].find("lemma=\"", 0);
      if (pos1 != string::npos) {
	pos1 = userData["w"].find(":", pos1) + 1;
	pos2 = userData["w"].find("\"", pos1) - 1;
	tagData = userData["w"].substr(pos1, pos2-pos1);
  buf.appendFormatted(" {\\fs15 <%s>}", tagData.c_str() );
      }
      pos1 = userData["w"].find("morph=\"", 0);
      if (pos1 != string::npos) {
	pos1 = userData["w"].find(":", pos1) + 1;
	pos2 = userData["w"].find("\"", pos1) - 1;
	tagData = userData["w"].substr(pos1, pos2-pos1);
  buf.appendFormatted(" {\\fs15 <%s>}", tagData.c_str() );
      }
      pos1 = userData["w"].find("POS=\"", 0);
      if (pos1 != string::npos) {
	pos1 = userData["w"].find(":", pos1) + 1;
	pos2 = userData["w"].find("\"", pos1) - 1;
	tagData = userData["w"].substr(pos1, pos2-pos1);
  buf.appendFormatted(" {\\fs15 <%s>}", tagData.c_str() );
      }
      
      buf += "}";
    }
    
    //p
    else if (!strncmp(token, "p", 1)) {
      buf += "{\\par\\par ";
    }
    else if (!strncmp(token, "/p", 2)) {
      buf += "}";
    }
    
    //reference
    else if (!strncmp(token, "reference", 8)) {
      buf += "{<a href=\"\">";
    }
    else if (!strncmp(token, "/reference", 9)) {
      buf += "</a>}";
    }
    
    //line
    else if (!strncmp(token, "line", 4)) {
      buf += "{\\par ";
    }
    else if (!strncmp(token, "/line", 5)) {
      buf += "}";
    }

    //note
    else if (!strncmp(token, "note", 4)) {
      buf += " {\\i1\\fs15 (";
    }
    else if (!strncmp(token, "/note", 5)) {
      buf += " ) }";
    }
    
    //title
    else if (!strncmp(token, "title", 5)) {
      buf += "{\\i1\\b1 ";
    }
    else if (!strncmp(token, "/title", 6)) {
      buf += "\\par}";
    }

    //hi
    else if (!strncmp(token, "hi", 2)) {
      tagData=token;
      pos1 = tagData.find("type=\"b", 0);
      if (pos1 != string::npos) {
	buf += "{\\b1 ";
      }
      else {
	buf += "{\\i1 ";
      }
    }
    else if (!strncmp(token, "/hi", 3)) {
      buf += "}";
    }

    //q
    else if (!strncmp(token, "q", 1)) {
      buf += "{";
      tagData=token;
      pos1 = tagData.find("who=\"", 0);
      if (pos1 != string::npos) {
	pos2 = tagData.find("\"", pos1);
	if (tagData.substr(pos1, pos2).find("Jesus", 0) != string::npos) {
	  buf += "\\cf6 ";
	}
      }
    }
    else if (!strncmp(token, "/q", 2)) {
      buf += "}";
    }

    //transChange
    else if (!strncmp(token, "transChange", 11)) {
      buf += "{\\i1 ";
    }
    else if (!strncmp(token, "/transChange", 12)) {
      buf += "}";
    }

    else {
      return false;  // we still didn't handle token
		}
  }
  return true;
}


SWORD_NAMESPACE_END
