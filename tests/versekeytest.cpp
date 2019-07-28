/******************************************************************************
 *
 *  versekeytest.cpp -
 *
 * $Id$
 *
 * Copyright 2007-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <swordxx/keys/listkey.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/localemgr.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;

using std::cout;
using std::endl;

int main(int /* argc */, char ** /* argv */) {
        VerseKey vk;
    VerseKey *parser = new VerseKey(); //(VerseKey *)mod->CreateKey();
    parser->setIntros(true);

    ListKey result = parser->parseVerseList("[ Testament 1 Heading ]");
    cout << "Should be: [ Testament 1 Heading ]\n" << result.getText() << "\n\n";

    parser->setText("[ Testament 1 Heading ]");
    cout << "Should be: [ Testament 1 Heading ]\n" << parser->getText() << "\n\n";

    result.clear();

    ListKey scope = parser->parseVerseList("amos 2:2", "", true);

    static auto const isSingle =
            [](auto & scope_) {
                auto oldScope(scope_);
                scope_.increment();
                return oldScope == scope_;
            };
    cout << (isSingle(scope) ? "single" : "multiple") << "\n";

    scope = parser->parseVerseList("amos", "", true);

    cout << (isSingle(scope) ? "single" : "multiple") << "\n";

    scope = parser->parseVerseList("amos", "", true);

    scope.increment();
    scope.increment();
    scope.increment();
    scope.increment();

    VerseKey *x = new VerseKey(); //(VerseKey *)mod->CreateKey();
    x->positionFrom(scope);
    x->clearBounds();

    std::cout << "x: " << x->getText() << "\n";

    result << *x;

    std::cout << result.getText() << "\n";

    result.positionToTop();

    std::cout << result.getText() << "\n";

     const char *bounds = "lk,acts";
     scope = parser->parseVerseList(bounds, "", true);

     VerseKey boundTest("lk", "acts");

     boundTest.setText("Is.1.13");
     std::cout << "Error: " << (int)boundTest.popError() << ": " << boundTest.getText() << "\n";
     boundTest.setText("1Sam.21.1");
     std::cout << "Error: " << (int)boundTest.popError() << ": " << boundTest.getText() << "\n";
     boundTest.setText("acts.5.1");
     std::cout << "Error: " << (int)boundTest.popError() << ": " << boundTest.getText() << "\n";
     boundTest.setText("rom.5.1");
     std::cout << "Error: " << (int)boundTest.popError() << ": " << boundTest.getText() << "\n";


     x->setText("Is.1.13");
     scope.setText(x->getText());
     const std::string T1 = scope.getText();
     const std::string T2 = x->getText();
     if (T1 == T2) std::cout << "Error restricting bounds: " << x->getText() << " is in " << bounds << "\n";

     if (!scope.popError()) std::cout << "Error restricting bounds: " << x->getText() << " is in " << bounds << "\n";

     x->setText("1Sam.21.1");
     scope.positionFrom(*x);
     if (!scope.popError()) std::cout << "Error restricting bounds: " << x->getText() << " is in " << bounds << "\n";

    cout << "\nNormalization on; headings on ====\n\n";

    vk.setAutoNormalize(true);
    vk.setIntros(true);

    vk.setText("jn3.50");
    cout << "jn.3.50: " << vk.getText() << "\n";
    vk.increment();
    cout << "++: " << vk.getText() << "\n";
    vk.decrement();
    cout << "--: " << vk.getText() << "\n";
    vk.positionToMaxVerse();
    cout << "MAXVERSE: " << vk.getText() << "\n";
    vk.positionToMaxChapter();
    cout << "MAXCHAPTER: " << vk.getText() << "\n";
    vk.positionToTop();
    cout << "TOP: " << vk.getText() << "\n";
    vk.positionToBottom();
    cout << "BOTTOM: " << vk.getText() << "\n";

    cout << "\nNormalization off; headings on ====\n\n";

    vk.setAutoNormalize(false);
    vk.setIntros(true);

    vk.setText("jn3.50");
    cout << "jn.3.50: " << vk.getText() << "\n";
    vk.increment();
    cout << "++: " << vk.getText() << "\n";
    vk.decrement();
    cout << "--: " << vk.getText() << "\n";
    vk.positionToMaxVerse();
    cout << "MAXVERSE: " << vk.getText() << "\n";
    vk.positionToMaxChapter();
    cout << "MAXCHAPTER: " << vk.getText() << "\n";
    vk.positionToTop();
    cout << "TOP: " << vk.getText() << "\n";
    vk.positionToBottom();
    cout << "BOTTOM: " << vk.getText() << "\n";

    cout << "\nNormalization on; headings off ====\n\n";

    vk.setAutoNormalize(true);
    vk.setIntros(false);

    vk.setText("jn3.50");
    cout << "jn.3.50: " << vk.getText() << "\n";
    vk.increment();
    cout << "++: " << vk.getText() << "\n";
    vk.decrement();
    cout << "--: " << vk.getText() << "\n";
    vk.positionToMaxVerse();
    cout << "MAXVERSE: " << vk.getText() << "\n";
    vk.positionToMaxChapter();
    cout << "MAXCHAPTER: " << vk.getText() << "\n";
    vk.positionToTop();
    cout << "TOP: " << vk.getText() << "\n";
    vk.positionToBottom();
    cout << "BOTTOM: " << vk.getText() << "\n";

    cout << "\nNormalization off; headings off ====\n\n";

    vk.setAutoNormalize(false);
    vk.setIntros(false);

    vk.setText("jn3.50");
    cout << "jn.3.50: " << vk.getText() << "\n";
    vk.increment();
    cout << "++: " << vk.getText() << "\n";
    vk.decrement();
    cout << "--: " << vk.getText() << "\n";
    vk.positionToMaxVerse();
    cout << "MAXVERSE: " << vk.getText() << "\n";
    vk.positionToMaxChapter();
    cout << "MAXCHAPTER: " << vk.getText() << "\n";
    vk.positionToTop();
    cout << "TOP: " << vk.getText() << "\n";
    vk.positionToBottom();
    cout << "BOTTOM: " << vk.getText() << "\n";

    VerseKey yo = "jn.3.16";
    VerseKey yo2(yo);
    yo.increment();
    cout << yo2.getText() << ": " << (int)yo2.popError() <<  endl;

    VerseKey vkey;
    VerseKey tmpkey = "1sam 1:1";
    vkey.setAutoNormalize(true);
    vkey = tmpkey;
    int chapter = (vkey.getChapter()-1);
    vkey.setChapter(chapter);

    cout << tmpkey.getText() << ": getChapter() - 1: " << vkey.getText() << endl;

    cout << "\nBook math\n\n";

    vkey.setText("Mark.1.1");
    vkey.decrement();
    cout << "Mark.1.1-- = " << vkey.getText() << "\n";
    vkey.increment();
    cout << "++ = " << vkey.getText() << "\n";
    vkey.setChapter(vkey.getChapter() - 1);
    cout << ".setChapter(.getChapter() - 1) = " << vkey.getText() << "\n";

    vkey.setText("Matthew.1.1");
    vkey.decrement();
    cout << "Matthew.1.1-- = " << vkey.getText() << "\n";
    vkey.increment();
    cout << "++ = " << vkey.getText() << "\n";
    vkey.setBook(vkey.getBook() - 1);
    cout << ".setBook(.getBook() - 1) = " << vkey.getText() << "\n";

    cout << "\nChapter math\n\n";

    cout << "Matthew.1.1 - 1 chapter\n";
    vkey.setText("Matthew.1.1");
    vkey.setChapter(vkey.getChapter() - 1);
    cout << ".setChapter(.getChapter() - 1) = " << vkey.getText() << "\n";

    cout << "\nVerse math\n\n";

    cout << "Matthew.1.1 - 1 verse\n";
    vkey.setText("Matthew.1.1");
    vkey.setVerse(vkey.getVerse() - 1);
    cout << ".setVerse(.getVerse() - 1) = " << vkey.getText() << "\n";

    return 0;
}
