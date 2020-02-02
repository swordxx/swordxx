/******************************************************************************
 *
 *  swmgrex.cpp -    Class SWMgr manages installed modules for a frontend.
 *            The developer may use this class to query what modules
 *            are installed and to retrieve an (SWModule *) for any
 *            one of these modules
 *
 *            SWMgr makes its modules available as an STL Map.
 *            The Map definition is typedef'ed as ModMap
 *            ModMap consists of:    FIRST : std::string moduleName
 *                              SECOND: SWModule *module
 *
 * Copyright 1998-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;
using namespace std;


int main() {

    SWMgr manager;        // create a default manager that looks in the current directory for mods.conf

    cout << "\nInstalled Modules:\n\n";

// Loop thru all installed modules and print out information

    for (auto const & mp : manager.modules()) {
        std::string const & modName  = mp.first;  // mod.conf section name (stored in module->Name())
        SWModule const & module  = *mp.second;
        cout << modName << "(" << module.getName() << ") | " << module.getType() << "\n";
    }

// Print out a verse from the first module:

    cout << "\n" << manager.modules().begin()->second->getKeyText() << ":\n";
    cout << manager.modules().begin()->second->renderText();
    cout << " (" << manager.modules().begin()->second->getName() << ")\n";

// Print out the same verse from the second module (less confusing):

    auto modIterator = manager.modules().begin();    // get first module
    modIterator++;                // increment to next module

    SWModule & mod = *modIterator->second;

    cout << "\n" << mod.getKeyText() << ":\n";
//    cout << (const char *)(*mod);        // we could do this, the same as above
    cout << mod.renderText();
    cout << " (" << mod.getName() << ")\n\n";

    return 0;

}
