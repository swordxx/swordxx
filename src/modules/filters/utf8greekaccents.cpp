/******************************************************************************
 *
 *  utf8greekaccents.cpp -	SWFilter descendant to remove UTF-8 Greek
 *				accents
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

#include <stdlib.h>
#include <stdio.h>
#include <utf8greekaccents.h>


#ifdef _ICU_
#include <utf8nfkd.h>
sword::UTF8NFKD decompose;
#endif


SWORD_NAMESPACE_START

namespace {

	static const char oName[] = "Greek Accents";
	static const char oTip[]  = "Toggles Greek Accents";

	static const StringList *oValues() {
		static const SWBuf choices[3] = {"On", "Off", ""};
		static const StringList oVals(&choices[0], &choices[2]);
		return &oVals;
	}
}


UTF8GreekAccents::UTF8GreekAccents() : SWOptionFilter(oName, oTip, oValues()) {
}


UTF8GreekAccents::~UTF8GreekAccents() {};


char UTF8GreekAccents::processText(SWBuf &text, const SWKey *key, const SWModule *module) {

	if (!option) { //we don't want greek accents
    		//unsigned char *to, *from;
		//to = (unsigned char*)text;
		//for (from = (unsigned char*)text; *from; from++) {
#ifdef _ICU_
		decompose.processText(text, (SWKey *)2);  // note the hack of 2 to mimic a real key. TODO: remove all hacks
#endif
		
		SWBuf orig = text;
		const unsigned char* from = (unsigned char*)orig.c_str();
		for (text = ""; *from; from++) {		
			//first just remove combining characters
			if (*from == 0xE2 && *(from + 1) == 0x80 && *(from + 2) == 0x99) {
				from += 2;
			}
			else if (*from == 0xCC && *(from + 1)) {
				if (*(from + 1) == 0x80 || *(from + 1) == 0x81 || *(from + 1) == 0x82 || *(from + 1) == 0x88 || *(from + 1) == 0x93 || *(from + 1) == 0x94) {
					from++;
				}
			}
			else if (*from == 0xCD && (*(from + 1) == 0xBA || *(from + 1) == 0x82)) {
				from++;
			}
			//now converted pre-composed characters to their alphabetic bases, discarding the accents

			//Greek
			//capital alpha
			else if ((*from == 0xCE && *(from + 1) == 0x86)) {
				text += 0xCE;
				text += 0x91;
				from++;
			}
			//capital epsilon
			else if ((*from == 0xCE && *(from + 1) == 0x88)) {
				text += 0xCE;
				text += 0x95;
				from++;
			}
			//capital eta
			else if ((*from == 0xCE && *(from + 1) == 0x89)) {
				text += 0xCE;
				text += 0x97;
				from++;
			}
			//capital iota
			else if ((*from == 0xCE && (*(from + 1) == 0x8A || *(from + 1) == 0xAA))) {
				text += 0xCE;
				text += 0x99;
				from++;
			}
			//capital omicron
			else if ((*from == 0xCE && *(from + 1) == 0x8C)) {
				text += 0xCE;
				text += 0x9F;
				from++;
			}
			//capital upsilon
			else if ((*from == 0xCE && (*(from + 1) == 0x8E || *(from + 1) == 0xAB))) {
				text += 0xCE;
				text += 0xA5;
				from++;
			}
			//capital omega
			else if ((*from == 0xCE && *(from + 1) == 0x8F)) {
				text += 0xCE;
				text += 0xA9;
				from++;
			}

			//alpha
			else if ((*from == 0xCE && *(from + 1) == 0xAC)) {
				text += 0xCE;
				text += 0xB1;
				from++;
			}
			//epsilon
			else if ((*from == 0xCE && *(from + 1) == 0xAD)) {
				text += 0xCE;
				text += 0xB5;
				from++;
			}
			//eta
			else if ((*from == 0xCE && *(from + 1) == 0xAE)) {
				text += 0xCE;
				text += 0xB7;
				from++;
			}
			//iota
			else if ((*from == 0xCE && *(from + 1) == 0xAF) || (*from == 0xCF && *(from + 1) == 0x8A)) {
				text += 0xCE;
				text += 0xB9;
				from++;
			}
			//omicron
			else if ((*from == 0xCF && *(from + 1) == 0x8C)) {
				text += 0xCE;
				text += 0xBF;
				from++;
			}
			//upsilon
			else if ((*from == 0xCE && *(from + 1) == 0x88) || (*from == 0xCF && (*(from + 1) == 0x8B || *(from + 1) == 0x8D))) {
				text += 0xCF;
				text += 0x85;
				from++;
			}
			//omega
			else if ((*from == 0xCF && *(from + 1) == 0x8E)) {
				text += 0xCF;
				text += 0x89;
				from++;
			}

			//Extended Greek
			//capital alpha
			else if (*from == 0xE1 && (((*(from + 1) == 0xBC || *(from + 1) == 0xBE) && *(from + 2) >= 0x88 && *(from + 2) <= 0x8F) || (*(from + 1) == 0xBE && *(from + 2) >= 0xB8 && *(from + 2) <= 0xBC))) {
				text += 0xCE;
				text += 0x91;
				from+=2;
			}
			//capital epsilon
			else if (*from == 0xE1 && ((*(from + 1) == 0xBC && *(from + 2) >= 0x98 && *(from + 2) <= 0x9D) || (*(from + 1) == 0xBF && (*(from + 2) == 0x88 || *(from + 2) == 0x89)))) {
				text += 0xCE;
				text += 0x95;
				from+=2;
			}
			//capital eta
			else if (*from == 0xE1 && ((*(from + 1) == 0xBC && *(from + 2) >= 0xA8 && *(from + 2) <= 0xAF) || (*(from + 1) == 0xBE && *(from + 2) >= 0x98 && *(from + 2) <= 0x9F) || (*(from + 1) == 0xBF && *(from + 2) >= 0x8A && *(from + 2) <= 0x8C))) {
				text += 0xCE;
				text += 0x97;
				from+=2;
			}
			//capital iota
			else if (*from == 0xE1 && ((*(from + 1) == 0xBC && *(from + 2) >= 0xB8 && *(from + 2) <= 0xBF) || (*(from + 1) == 0xBF && *(from + 2) >= 0x98 && *(from + 2) <= 0x9B))) {
				text += 0xCE;
				text += 0x99;
				from+=2;
			}
			//capital omicron
			else if (*from == 0xE1 && (((*(from + 1) == 0xBD && *(from + 2) >= 0x88 && *(from + 2) <= 0x8D)) || ((*(from + 1) == 0xBF && (*(from + 2) == 0xB8 || *(from + 2) == 0xB9))))) {
				text += 0xCE;
				text += 0x9F;
				from+=2;
			}
			//capital upsilon
			else if (*from == 0xE1 && ((*(from + 1) == 0xBD && *(from + 2) >= 0x99 && *(from + 2) <= 0x9F) || (*(from + 1) == 0xBF && *(from + 2) >= 0xA8 && *(from + 2) <= 0xAB))) {
				text += 0xCE;
				text += 0xA5;
				from+=2;
			}
			//capital omega
			else if (*from == 0xE1 && (((*(from + 1) == 0xBD || *(from + 1) == 0xBE) && *(from + 2) >= 0xA8 && *(from + 2) <= 0xAF) || (*(from + 1) == 0xBF && *(from + 2) >= 0xBA && *(from + 2) <= 0xBC))) {
				text += 0xCE;
				text += 0xA9;
				from+=2;
			}
			//capital rho
			else if (*from == 0xE1 && *(from + 1) == 0xBF && *(from + 2) == 0xAC) {
				text += 0xCE;
				text += 0xA1;
				from+=2;
			}

			//alpha
			else if (*from == 0xE1 && (
                            ((*(from + 1) == 0xBC || *(from + 1) == 0xBE) && *(from + 2) >= 0x80 && *(from + 2) <= 0x87)
                         || (*(from + 1) == 0xBD && (*(from + 2) == 0xB0 || *(from + 2) == 0xB1))
                         || (*(from + 1) == 0xBE && *(from + 2) >= 0xB0 && *(from + 2) <= 0xB7))) {
				text += 0xCE;
				text += 0xB1;
				from+=2;
			}
			//epsilon
			else if (*from == 0xE1 && ((*(from + 1) == 0xBC && *(from + 2) >= 0x90 && *(from + 2) <= 0x95) || (*(from + 1) == 0xBD && (*(from + 2) == 0xB2 || *(from + 2) == 0xB3)))) {
				text += 0xCE;
				text += 0xB5;
				from+=2;
			}
			//eta
			else if (*from == 0xE1 && ((*(from + 1) == 0xBE && *(from + 2) >= 0x90 && *(from + 2) <= 0x97) || (*(from + 1) == 0xBC && *(from + 2) >= 0xA0 && *(from + 2) <= 0xA7) || (*(from + 1) == 0xBF && *(from + 2) >= 0x82 && *(from + 2) <= 0x87) || (*(from + 1) == 0xBD && (*(from + 2) == 0xB4 || *(from + 2) == 0xB5)))) {
				text += 0xCE;
				text += 0xB7;
				from+=2;
			}
			//iota
			else if (*from == 0xE1 && ((*(from + 1) == 0xBC && *(from + 2) >= 0xB0 && *(from + 2) <= 0xB7) || (*(from + 1) == 0xBD && (*(from + 2) == 0xB6 || *(from + 2) == 0xB7)) || (*(from + 1) == 0xBF && *(from + 2) >= 0x90 && *(from + 2) <= 0x97))) {
				text += 0xCE;
				text += 0xB9;
				from+=2;
			}
			//omicron
			else if (*from == 0xE1 && (*(from + 1) == 0xBD && ((*(from + 2) >= 0x80 && *(from + 2) <= 0x85) || (*(from + 2) == 0xB8 || *(from + 2) == 0xB9)))) {
				text += 0xCE;
				text += 0xBF;
				from+=2;
			}
			//upsilon
			else if (*from == 0xE1 && ((*(from + 1) == 0xBD && ((*(from + 2) >= 0x90 && *(from + 2) <= 0x97) || *(from + 2) == 0xBA || *(from + 2) == 0xBB)) || (*(from + 1) == 0xBF && ((*(from + 2) >= 0xA0 && *(from + 2) <= 0xA3) || *(from + 2) == 0xA6 || *(from + 2) == 0xA7)))) {
				text += 0xCF;
				text += 0x85;
				from+=2;
			}
			//omega
			else if (*from == 0xE1 && ((*(from + 1) == 0xBD && ((*(from + 2) >= 0xA0 && *(from + 2) <= 0xA7) || (*(from + 2) == 0xBC || *(from + 2) == 0xBD))) || (*(from + 1) == 0xBE && (*(from + 2) >= 0xA0 && *(from + 2) <= 0xA7)) || (*(from + 1) == 0xBF && *(from + 2) >= 0xB2 && *(from + 2) <= 0xB7))) {
				text += 0xCF;
				text += 0x89;
				from+=2;
			}
			//rho
			else if (*from == 0xE1 && *(from + 1) == 0xBF && (*(from + 2) == 0xA4 && *(from + 2) == 0xA5)) {
				text += 0xCF;
				text += 0x81;
				from+=2;
			}
			else { //no characters we filter
				text += *from;
			}
		}
	}
	return 0;
}






SWORD_NAMESPACE_END
