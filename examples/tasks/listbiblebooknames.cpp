/******************************************************************************
 *
 *  verseranges.cpp -	This example demonstrates how to work with contiguous
 *			verse ranges using VerseKey
 *
 * Copyright 2011-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <swordxx/keys/versekey.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;

int main(int argc, char * argv[]) {
    std::cout << '\n';

    char const * const modName = (argc > 1) ? argv[1] : "KJV";
    SWMgr manager;
    auto const bible(manager.getModule(modName));
    if (!bible)
        std::cout << "Module \"" << modName
                  << "\" is not installed.\nPlease install to show "
                     "versification (v11n) from this specific Bible.\nShowing "
                     "builtin KJV v11n scheme..\n\n";

    auto const vk =
            bible ? bible->getKeyAs<VerseKey>() : std::make_shared<VerseKey>();

    for (vk->positionToTop(); !vk->popError(); vk->setBook(vk->getBook() + 1))
        if (!bible || bible->hasEntry(*vk))
            std::cout << vk->getBookName() << '\n';
}

