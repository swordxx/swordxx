/******************************************************************************
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

#include <iostream>
#include <string>
#include <swordxx/keys/listkey.h>
#include <swordxx/keys/versekey.h>


int main() {
    swordxx::VerseKey parser;
    parser.setIntros(true);

    std::cout << "Should be: [ Testament 1 Heading ]\n"
              << parser.parseVerseList("[ Testament 1 Heading ]").getText()
              << "\n\n";

    parser.setText("[ Testament 1 Heading ]");
    std::cout << "Should be: [ Testament 1 Heading ]\n" << parser.getText()
              << "\n\n";

    {
        auto const singleTest =
                [&parser](char const * const location) {
                    auto scope(parser.parseVerseList(location, "", true));
                    auto const oldScope(scope);
                    scope.increment();
                    std::cout << ((oldScope == scope)
                                  ? "single\n"
                                  : "multiple\n");
                };
        singleTest("amos 2:2");
        singleTest("amos");
    }

    swordxx::VerseKey x;
    {
        auto scope(parser.parseVerseList("amos", "", true));
        scope.increment();
        scope.increment();
        scope.increment();
        scope.increment();
        x.positionFrom(scope);
    }
    x.clearBounds();

    std::cout << "x: " << x.getText() << '\n';

    swordxx::ListKey result;
    result << x;

    std::cout << result.getText() << '\n';

    result.positionToTop();

    std::cout << result.getText() << '\n';

    swordxx::VerseKey boundTest("lk", "acts");
    boundTest.setText("Is.1.13");
    std::cout << "Error: " << static_cast<int>(boundTest.popError()) << ": "
              << boundTest.getText() << '\n';
    boundTest.setText("1Sam.21.1");
    std::cout << "Error: " << static_cast<int>(boundTest.popError()) << ": "
              << boundTest.getText() << '\n';
    boundTest.setText("acts.5.1");
    std::cout << "Error: " << static_cast<int>(boundTest.popError()) << ": "
              << boundTest.getText() << '\n';
    boundTest.setText("rom.5.1");
    std::cout << "Error: " << static_cast<int>(boundTest.popError()) << ": "
              << boundTest.getText() << '\n';


    x.setText("Is.1.13");
    {
        auto const bounds = "lk,acts";
        auto scope(parser.parseVerseList(bounds, "", true));
        scope.setText(x.getText());
        if (scope.getText() == x.getText())
            std::cout << "Error restricting bounds: " << x.getText()
                      << " is in " << bounds << '\n';

        if (!scope.popError())
            std::cout << "Error restricting bounds: " << x.getText()
                      << " is in " << bounds << '\n';

        x.setText("1Sam.21.1");
        scope.positionFrom(x);
        if (!scope.popError())
            std::cout << "Error restricting bounds: " << x.getText()
                      << " is in " << bounds << '\n';
    }

    std::cout << "\nNormalization on; headings on ====\n\n";

    swordxx::VerseKey vk;
    vk.setAutoNormalize(true);
    vk.setIntros(true);

    vk.setText("jn3.50");
    std::cout << "jn.3.50: " << vk.getText() << '\n';
    vk.increment();
    std::cout << "++: " << vk.getText() << '\n';
    vk.decrement();
    std::cout << "--: " << vk.getText() << '\n';
    vk.positionToMaxVerse();
    std::cout << "MAXVERSE: " << vk.getText() << '\n';
    vk.positionToMaxChapter();
    std::cout << "MAXCHAPTER: " << vk.getText() << '\n';
    vk.positionToTop();
    std::cout << "TOP: " << vk.getText() << '\n';
    vk.positionToBottom();
    std::cout << "BOTTOM: " << vk.getText() << '\n';

    std::cout << "\nNormalization off; headings on ====\n\n";

    vk.setAutoNormalize(false);
    vk.setIntros(true);

    vk.setText("jn3.50");
    std::cout << "jn.3.50: " << vk.getText() << '\n';
    vk.increment();
    std::cout << "++: " << vk.getText() << '\n';
    vk.decrement();
    std::cout << "--: " << vk.getText() << '\n';
    vk.positionToMaxVerse();
    std::cout << "MAXVERSE: " << vk.getText() << '\n';
    vk.positionToMaxChapter();
    std::cout << "MAXCHAPTER: " << vk.getText() << '\n';
    vk.positionToTop();
    std::cout << "TOP: " << vk.getText() << '\n';
    vk.positionToBottom();
    std::cout << "BOTTOM: " << vk.getText() << '\n';

    std::cout << "\nNormalization on; headings off ====\n\n";

    vk.setAutoNormalize(true);
    vk.setIntros(false);

    vk.setText("jn3.50");
    std::cout << "jn.3.50: " << vk.getText() << '\n';
    vk.increment();
    std::cout << "++: " << vk.getText() << '\n';
    vk.decrement();
    std::cout << "--: " << vk.getText() << '\n';
    vk.positionToMaxVerse();
    std::cout << "MAXVERSE: " << vk.getText() << '\n';
    vk.positionToMaxChapter();
    std::cout << "MAXCHAPTER: " << vk.getText() << '\n';
    vk.positionToTop();
    std::cout << "TOP: " << vk.getText() << '\n';
    vk.positionToBottom();
    std::cout << "BOTTOM: " << vk.getText() << '\n';

    std::cout << "\nNormalization off; headings off ====\n\n";

    vk.setAutoNormalize(false);
    vk.setIntros(false);

    vk.setText("jn3.50");
    std::cout << "jn.3.50: " << vk.getText() << '\n';
    vk.increment();
    std::cout << "++: " << vk.getText() << '\n';
    vk.decrement();
    std::cout << "--: " << vk.getText() << '\n';
    vk.positionToMaxVerse();
    std::cout << "MAXVERSE: " << vk.getText() << '\n';
    vk.positionToMaxChapter();
    std::cout << "MAXCHAPTER: " << vk.getText() << '\n';
    vk.positionToTop();
    std::cout << "TOP: " << vk.getText() << '\n';
    vk.positionToBottom();
    std::cout << "BOTTOM: " << vk.getText() << '\n';

    {
        swordxx::VerseKey yo("jn.3.16");
        auto yo2(yo);
        yo.increment();
        std::cout << yo2.getText() << ": " << static_cast<int>(yo2.popError())
                  << std::endl;
    }

    swordxx::VerseKey vkey;
    vkey.setAutoNormalize(true);
    {
        swordxx::VerseKey tmpkey("1sam 1:1");
        vkey = tmpkey;
        vkey.setChapter(vkey.getChapter() - 1);

        std::cout << tmpkey.getText() << ": getChapter() - 1: "
                  << vkey.getText() << std::endl;
    }

    std::cout << "\nBook math\n\n";

    vkey.setText("Mark.1.1");
    vkey.decrement();
    std::cout << "Mark.1.1-- = " << vkey.getText() << '\n';
    vkey.increment();
    std::cout << "++ = " << vkey.getText() << '\n';
    vkey.setChapter(vkey.getChapter() - 1);
    std::cout << ".setChapter(.getChapter() - 1) = " << vkey.getText() << '\n';

    vkey.setText("Matthew.1.1");
    vkey.decrement();
    std::cout << "Matthew.1.1-- = " << vkey.getText() << '\n';
    vkey.increment();
    std::cout << "++ = " << vkey.getText() << '\n';
    vkey.setBook(vkey.getBook() - 1);
    std::cout << ".setBook(.getBook() - 1) = " << vkey.getText() << '\n';

    std::cout << "\nChapter math\n\n";

    std::cout << "Matthew.1.1 - 1 chapter\n";
    vkey.setText("Matthew.1.1");
    vkey.setChapter(vkey.getChapter() - 1);
    std::cout << ".setChapter(.getChapter() - 1) = " << vkey.getText() << '\n';

    std::cout << "\nVerse math\n\n";

    std::cout << "Matthew.1.1 - 1 verse\n";
    vkey.setText("Matthew.1.1");
    vkey.setVerse(vkey.getVerse() - 1);
    std::cout << ".setVerse(.getVerse() - 1) = " << vkey.getText() << '\n';
}
