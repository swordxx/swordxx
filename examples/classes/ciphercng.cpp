/******************************************************************************
 *
 *  ciphercng.cpp -    This example demonstrates how to change the cipher key
 *            of a module. The change is only in effect for this
 *            run.  This DOES NOT change the cipherkey in the
 *            module's .conf file.
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

#include <cstdio>
#include <iostream>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;
using namespace std;


int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "usage: %s <modName>\n", *argv);
        std::exit(-1);
    }

    SWMgr manager;        // create a default manager that looks in the current directory for mods.conf
    auto const it = manager.modules().find(argv[1]);
    if (it == manager.modules().end()) {
        fprintf(stderr, "%s: couldn't find module: %s\n", *argv, argv[1]);
        std::exit(-1);
    }

    SWModule & module = *it->second;
    string key;

    cout << "\nPress [CTRL-C] to end\n\n";
    while (true) {
        cout << "\nModule text:\n";
        module.setKey("1jn 1:9");
        cout << "[ " << module.getKeyText() << " ]\n";
        cout << module.renderText();
        cout << "\n\nEnter new cipher key: ";
        cin >> key;
        cout << "\nSetting key to: " << key;
        manager.setCipherKey(argv[1], key.c_str());
    }

    return 0;
}
