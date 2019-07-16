/******************************************************************************
 *
 *  outvplskeleton.cpp -This example shows how to output a skeleton of
 *			verse references from a SWORD module.
 *
 * Gen.1.1
 * Gen.1.2
 * ...
 *
 * Class SWMgr manages installed modules for a frontend.
 * The developer may use this class to query what modules are installed
 * and to retrieve an (SWModule *) for any one of these modules
 *
 * SWMgr makes its modules available as an STL Map.
 * The Map definition is typedef'ed as ModMap
 * ModMap consists of: FIRST : SWBuf moduleName
 *                     SECOND: SWModule *module
 *
 * Copyright 2008-2013 CrossWire Bible Society (http://www.crosswire.org)
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
/******************************************************************************
 *
 * Copyright Jaak Ristioja
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

#include <cstdlib>
#include <iostream>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


int main(int argc, char **argv) {
    swordxx::SWMgr manager(
                std::make_shared<swordxx::MarkupFilterMgr>(swordxx::FMT_XHTML,
                                                           swordxx::ENC_UTF8));

    auto const * bookName = (argc > 1) ? argv[1] : "KJV";
    auto book(manager.getModule(bookName));
    if (!book)
        return EXIT_FAILURE;

    book->setProcessEntryAttributes(false);
    auto const vk(book->getKeyAs<swordxx::VerseKey const>());
    book->positionToTop();
    if (book->popError())
        return EXIT_FAILURE;
    while (book->getRawEntry().empty()) {
         book->increment();
         if (book->popError())
             return EXIT_FAILURE;
    }
    for (;;) {
        std::cout << (vk ? vk->getOSISRef() : book->getKeyText()) << '\n';
        book->increment();
        if (book->popError())
            break;
    }
}
