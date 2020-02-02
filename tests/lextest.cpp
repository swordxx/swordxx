/******************************************************************************
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

#include <iostream>
#include <swordxx/modules/lexdict/rawld.h>


using namespace swordxx;
using std::cout;
using std::endl;

int main(int /* argc */, char ** /* argv */)
{
    RawLD::createModule("tmp/lextest");
    RawLD lex("tmp/lextest");

    lex.setKey("b");
    lex.setEntry("entry for b");

    lex.setKey("a");
    lex.setEntry("entry for a");

    lex.positionToTop();
    cout << lex.getKeyText() << endl;
    lex.increment();
    cout << lex.getKeyText() << endl;

    lex.setKey("a");
    lex.deleteEntry();

//    lex.setKey("a");
//    lex << "y";

    lex.positionToBottom();
    return 0;
}
