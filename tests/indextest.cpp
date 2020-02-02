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
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;

int main(int /* argc */, char ** /* argv */) {
    SWMgr mymgr;

    if (auto const bbe = mymgr.getModule("BBE")) {
        auto vk(std::make_shared<VerseKey>());
        bbe->setKey(vk);
        for (; !bbe->popError(); (*bbe).increment()) {
            std::cout << vk->getIndex() << std::endl;
        }
    }
    return 0;
}
