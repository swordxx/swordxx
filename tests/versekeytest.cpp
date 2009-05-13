/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <versekey.h>
#include <listkey.h>
#include <localemgr.h>
#include <swmgr.h>
#include <swmodule.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

using std::cout;
using std::endl;

int main(int argc, char **argv) {
/*
VerseKey currentVerse;
currentVerse.AutoNormalize(0);
currentVerse.Headings(1);
currentVerse.Persist(1);
currentVerse = "jn2";
cout << currentVerse << endl;

	SWMgr mgr;
	SWModule *mod = mgr.getModule("KJVgb");
*/
	VerseKey *parser = new VerseKey(); //(VerseKey *)mod->CreateKey();

	ListKey scope = parser->ParseVerseList("amos 2:2", *parser, true);

	cout << ((scope++ == scope) ? "single" : "multiple") << "\n";

	scope = parser->ParseVerseList("amos", *parser, true);

	cout << ((scope++ == scope) ? "single" : "multiple") << "\n";

	scope = parser->ParseVerseList("amos", *parser, true);

	ListKey result;

	scope++;
	scope++;
	scope++;
	scope++;

	VerseKey *x = new VerseKey(); //(VerseKey *)mod->CreateKey();
	*x = scope;
	x->clearBound();

	std::cout << "x: " << x->getText() << "\n";

	result << *x;

	std::cout << result.getText() << "\n";

	result = TOP;

	std::cout << result.getText() << "\n";

     const char *bounds = "lk,acts";
     scope = parser->ParseVerseList(bounds, *parser, true);

     VerseKey boundTest("lk", "acts");

     boundTest.setText("Is.1.13");
     std::cout << "Error: " << (int)boundTest.Error() << ": " << boundTest << "\n";
     boundTest.setText("1Sam.21.1");
     std::cout << "Error: " << (int)boundTest.Error() << ": " << boundTest << "\n";
     boundTest.setText("acts.5.1");
     std::cout << "Error: " << (int)boundTest.Error() << ": " << boundTest << "\n";
     boundTest.setText("rom.5.1");
     std::cout << "Error: " << (int)boundTest.Error() << ": " << boundTest << "\n";


     *x = "Is.1.13";
     scope = *x;
//     if (scope == x) std::cout << "Error restricting bounds: " << x.getText() << " is in " << bounds << "\n";
     if (!scope.Error()) std::cout << "Error restricting bounds: " << x->getText() << " is in " << bounds << "\n";

     *x = "1Sam.21.1";
     scope = *x;
     if (!scope.Error()) std::cout << "Error restricting bounds: " << x->getText() << " is in " << bounds << "\n";

/*
	VerseKey *y = (VerseKey *)mod->CreateKey();
	(*y) = "lev 1.1";
	cout << (*y) << "\n";
	(*y)++;
	cout << (*y) << "\n";
	(*y)--;
	cout << (*y) << "\n";
	(*y)--;
	cout << (*y) << "\n";

	mod->setKey("Ruth 1.1");
	cout << mod->getKeyText() << "\n";
	(*mod)++;
	cout << mod->getKeyText() << "\n";
	(*mod)--;
	cout << mod->getKeyText() << "\n";
*/
	VerseKey vk;

	cout << "\nNormalization on; headings on ====\n\n";

	vk.AutoNormalize(1);
	vk.Headings(1);

	vk = "jn3.50";
/* TODO: these report 1 number off from what I think they should report 
	cout << "jn.3.50: " << vk << "\n";
	vk++;
	cout << "++: " << vk << "\n";
	vk--;
	cout << "--: " << vk << "\n";
*/
	vk = MAXVERSE;
	cout << "MAXVERSE: " << vk << "\n";
	vk = MAXCHAPTER;
	cout << "MAXCHAPTER: " << vk << "\n";
	vk = TOP;
	cout << "TOP: " << vk << "\n";
	vk = BOTTOM;
	cout << "BOTTOM: " << vk << "\n";

	cout << "\nNormalization off; headings on ====\n\n";
	
	vk.AutoNormalize(0);
	vk.Headings(1);

	vk = "jn3.50";
	cout << "jn.3.50: " << vk << "\n";
/* TODO: These probably are undefined and likely to be ok
	vk++;
	cout << "++: " << vk << "\n";
	vk--;
	cout << "--: " << vk << "\n";
	vk = MAXVERSE;
	cout << "MAXVERSE: " << vk << "\n";
*/
	vk = MAXCHAPTER;
	cout << "MAXCHAPTER: " << vk << "\n";
	vk = TOP;
	cout << "TOP: " << vk << "\n";
	vk = BOTTOM;
	cout << "BOTTOM: " << vk << "\n";

	cout << "\nNormalization on; headings off ====\n\n";
	
	vk.AutoNormalize(1);
	vk.Headings(0);

	vk = "jn3.50";
	cout << "jn.3.50: " << vk << "\n";
	vk++;
	cout << "++: " << vk << "\n";
	vk--;
	cout << "--: " << vk << "\n";
	vk = MAXVERSE;
	cout << "MAXVERSE: " << vk << "\n";
	vk = MAXCHAPTER;
	cout << "MAXCHAPTER: " << vk << "\n";
	vk = TOP;
	cout << "TOP: " << vk << "\n";
	vk = BOTTOM;
	cout << "BOTTOM: " << vk << "\n";

	cout << "\nNormalization off; headings off ====\n\n";
	
	vk.AutoNormalize(0);
	vk.Headings(0);

	vk = "jn3.50";
	cout << "jn.3.50: " << vk << "\n";
/* TODO: These probably are undefined and likely to be ok
	vk++;
	cout << "++: " << vk << "\n";
	vk--;
	cout << "--: " << vk << "\n";
	vk = MAXVERSE;
	cout << "MAXVERSE: " << vk << "\n";
*/
	vk = MAXCHAPTER;
	cout << "MAXCHAPTER: " << vk << "\n";
	vk = TOP;
	cout << "TOP: " << vk << "\n";
	vk = BOTTOM;
	cout << "BOTTOM: " << vk << "\n";

	VerseKey yo = "jn.3.16";
	VerseKey yo2 = yo++;
	cout << yo2 << ": " << (int)yo2.Error() <<  endl;

	VerseKey vkey;
	VerseKey tmpkey = "1sam 1:1";
	vkey.AutoNormalize(1);                          // line 147
	vkey = tmpkey;
	int chapter = (vkey.Chapter()-1);
	vkey.Chapter(chapter);

	cout << tmpkey << ": Chapter() - 1: " << vkey << endl;


	return 0;
}
