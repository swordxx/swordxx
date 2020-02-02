/******************************************************************************
 *
 * Copyright 1996-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/keys/versekey.h>
#include <swordxx/localemgr.h>
#include <swordxx/modules/comments/rawcom.h>
#include <swordxx/modules/texts/rawtext.h>
#include <swordxx/swtext.h>


using namespace swordxx;

//class EchoMod final: public SWText {

//public: /* Methods: */

//    inline EchoMod()
//        : SWText("echomod", "Echos back key")
//    {}

//    inline std::string & getRawEntryBuf() const final override {
//        m_retVal = *key;
//        return m_retVal;
//    }

//    mutable std::string m_retVal;

//};

int main(int argc, char **argv)
{
    int loop;
    int max;

    VerseKey yo("Gen.1.1", "Gen.1.2");
    std::cout << yo.getRangeText();
    if (argc > 1)
        LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(argv[1]);

    VerseKey    bla;
    long index;


    if (argc < 2) {
        bla.setText("James    1:19");
    } else {
        bla.setText(argv[2]);
    }

    std::cout << "\n Headings: " << (bool)bla.isIntros() << "\n";
    std::cout << " (.Index(Index()+1))\n";

    max = (argc < 4) ? 10 : std::atoi(argv[3]);

    for (loop = 0; loop < max; loop++) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")";
        bla.setIndex(index+1);
        std::cout << "-> " << bla.getText() << "\n";
    }

    std::cout << "-----------------\n";
    std::cout << "\n (.Index(Index()-1))\n";
    if (argc < 2) {
        bla.setText("James    1:19");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = max; loop; loop--) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")";
        bla.setIndex(index-1);
        std::cout << "-> " << bla.getText() << "\n";
    }

    std::cout << "-----------------\n";
    bla.setIntros(true);
    std::cout << "\n Headings: " << (bool)bla.isIntros() << "\n";
    std::cout << " key++\n";

    if (argc < 2) {
        bla.setText("Matthew  1:5");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = 0; loop < max && !bla.popError(); loop++, bla.increment()) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")\n";
    }

    std::cout << "-----------------\n";
    bla.setIntros(true);
    std::cout << "\n Headings: " << (bool)bla.isIntros() << "\n";
    std::cout << " key--\n";

    if (argc < 2) {
        bla.setText("Matthew  1:5");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = max; loop && !bla.popError(); loop--, bla.decrement()) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")\n";
    }

    std::cout << "-----------------\n";

    if (argc < 2) {
        bla.setText("Genesis  1:5");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = max; loop; loop--, bla.decrement()) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")\n";
    }

    std::cout << "-----------------\n";

    if (argc < 2) {
        bla.setText("Malachi  4:2");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = max; loop; loop--, bla.increment()) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")\n";
    }

    std::cout << "-----------------\n";

    if (argc < 2) {
        bla.setText("Revelation of John  22:17");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = max; loop; loop--, bla.increment()) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")\n";
    }


    std::cout << "-----------------\n";
    std::cout << "-------- Headings ---------\n";

    bla.setIntros(true);

    if (argc < 2) {
        bla.setText("Matthew  1:5");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = max; loop; loop--, bla.decrement()) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")\n";
    }

    std::cout << "-----------------\n";

    if (argc < 2) {
        bla.setText("Genesis  1:5");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = max; loop; loop--, bla.decrement()) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")\n";
    }

    std::cout << "-----------------\n";

    if (argc < 2) {
        bla.setText("Malachi  4:2");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = max; loop; loop--, bla.increment()) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")\n";
    }

    std::cout << "-----------------\n";

    if (argc < 2) {
        bla.setText("Revelation of John  22:17");
    } else {
        bla.setText(argv[2]);
    }

    for (loop = max; loop; loop--, bla.increment()) {
        index = bla.getIndex();
        std::cout << bla.getText() << "(" << index << ")\n";
    }

    std::cout << "\n\n";

    std::cout << "-------- Error Check ------------\n\n";
    bla.setText("Revelation of John 23:19");
    std::cout << "bla = \"Revelation of John 23:19\"\n";
    std::cout << "bla.getText() = " << bla.getText() << "\n";
    std::cout << "bla.popError() = " << (int)bla.popError() << " \n";
    std::cout << "bla++ \n";
    bla.increment();
    std::cout << "bla.popError() = " << (int)bla.popError() << " \n";

    bla.setIntros(false);
    for (bla.positionToBottom(); !bla.popError(); bla.setBook(bla.getBook()-1))
        std::cout << bla.getText() << "\n";
    bla.setTestament(1);
    bla.positionToBottom();
    std::cout << bla.getTestamentIndex() << "\n";
    std::cout << bla.getIndex() << "\n";
    std::cout << bla.getText() << "\n";
    bla.setTestament(2);
    bla.positionToBottom();
    std::cout << bla.getTestamentIndex() << "\n";
    std::cout << bla.getIndex() << "\n";
    std::cout << bla.getText() << "\n";
    return 0;
}
