/******************************************************************************
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/keys/versekey.h>
#include <swordxx/modules/texts/rawtext.h>
#include <swordxx/swmgr.h>


using namespace swordxx;

int main(int /* argc */, char ** /* argv */) {
    SWMgr mymgr;

    RawText::createModule(".");
    RawText mod(".");

    auto vk(std::make_shared<VerseKey>());
    vk->setIntros(true);
    vk->setAutoNormalize(false);
    mod.setKey(vk);

    vk->setVerse(0);
    vk->setChapter(0);
    vk->setBook(0);
    vk->setTestament(0);

    mod.setEntry("Module heading text");

    vk->setVerse(0);
    vk->setChapter(0);
    vk->setBook(0);
    vk->setTestament(1);

    mod.setEntry("OT heading text");

    vk->setTestament(1);
    vk->setBook(1);
    vk->setChapter(0);
    vk->setVerse(0);

    mod.setEntry("Gen heading text");

    vk->setTestament(1);
    vk->setBook(1);
    vk->setChapter(1);
    vk->setVerse(0);

    mod.setEntry("Gen 1 heading text");

    vk->setTestament(1);
    vk->setBook(1);
    vk->setChapter(1);
    vk->setVerse(1);

    mod.setEntry("Gen 1:1 text");


    vk->setTestament(0);
    vk->setBook(0);
    vk->setChapter(0);
    vk->setVerse(0);

    std::cout << "Module heading text ?= " << mod.renderText() << std::endl;

    vk->setTestament(1);
    vk->setBook(0);
    vk->setChapter(0);
    vk->setVerse(0);

    std::cout << "OT heading text ?= " << mod.renderText() << std::endl;

    vk->setTestament(1);
    vk->setBook(1);
    vk->setChapter(0);
    vk->setVerse(0);

    std::cout << "Gen heading text ?= " << mod.renderText() << std::endl;

    vk->setTestament(1);
    vk->setBook(1);
    vk->setChapter(1);
    vk->setVerse(0);

    std::cout << "Gen 1 heading text ?= " << mod.renderText() << std::endl;

    vk->setTestament(1);
    vk->setBook(1);
    vk->setChapter(1);
    vk->setVerse(1);

    std::cout << "Gen 1:1 text ?= " << mod.renderText() << std::endl;

      /* old introtest
    SWModule *mhc = mymgr.Modules["MHC"];

    if (mhc) {
        VerseKey vk;
        vk->setIntros(true);
        vk->setAutoNormalize(false);
        *vk = "jas 0:0";
        std::cout << *vk << ":\n";
        mhc->setKey(vk);
        std::cout << (const char *) mhc->Key() << ":\n";
        std::cout << (const char *) *mhc << "\n";
    }
      */
    return 0;
}


