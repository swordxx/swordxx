/******************************************************************************
 *
 *  bibliotest.cpp -
 *
 * $Id$
 *
 * Copyright 2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/swmgr.h>
#include <swordxx/swtext.h>


using namespace swordxx;

int main(int argc, char **argv) {
    SWMgr mymgr;
    if ( argc > 1 ) {
        if (SWModule * const module = mymgr.getModule(argv[1]))
            std::cout << module->getBibliography() << "\n";
        return 0;
    }
    else {
        std::cout << "Usage: bibliography <ModuleName>"<< "\n";
        return 1;
    }
}
