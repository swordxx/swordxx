/******************************************************************************
 *
 *  treekey.cpp -    code for class 'versekey'- a standard Biblical
 *            verse key
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "treekey.h"

#include <cstring>
#include "../utilstr.h"


namespace swordxx {

void TreeKey::init() {
    m_unsnappedKeyText = "";
    m_posChangeListener = nullptr;
}


void TreeKey::assureKeyPath(const char *keyBuffer) {

    if (!keyBuffer)
        keyBuffer = m_unsnappedKeyText.c_str();

    char * keybuf = nullptr;
    stdstr(&keybuf, keyBuffer);

    root();

    // TODO: change to NOT use strtok. strtok is dangerous.
    std::string tok = strtok(keybuf, "/");
    trimString(tok);
    while (!tok.empty()) {
        bool foundkey = false;
        if (hasChildren()) {
            firstChild();
            if (tok == getLocalName()) {
                foundkey = true;
            }
            else {
                while (nextSibling()) {
                    auto const & localName = getLocalName();
                    if (!localName.empty() && tok == localName) {
                        foundkey = true;
                        break;
                    }
                }
            }
            if (!foundkey) {
                append();
                setLocalName(std::move(tok));
                save();
            }
        }
        else {
            appendChild();
            setLocalName(std::move(tok));
            save();
        }

#ifdef DEBUG
//      std::cout << getLocalName() << " : " << tok << std::endl;
#endif

        tok = strtok(nullptr, "/");
        trimString(tok);

    }
    delete [] keybuf;
}


} /* namespace swordxx */
