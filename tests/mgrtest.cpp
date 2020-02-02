/******************************************************************************
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/swconfig.h>
#include <swordxx/swlog.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;

int main(int argc, char **argv) {
    std::cerr << "\n";
    SWLog::getSystemLog()->setLogLevel(SWLog::LOG_DEBUG);

    std::unique_ptr<SWConfig> sysConf;
    if (argc > 1)
        sysConf = std::make_unique<SWConfig>(argv[1]);

    SWMgr mymgr(nullptr, sysConf.get());
    std::cerr << "\n\nprefixPath: " << mymgr.prefixPath();
    std::cerr << "\nconfigPath: " << mymgr.m_configPath << "\n\n";

    for (auto const & mp : mymgr.modules()) {
        auto & module = *mp.second;
        std::cout << "[" << module.getName() << "] (Writable: " << (module.isWritable()?"Yes":"No") << ") [" << module.getDescription() << "]\n";
        std::cout << "AbsoluteDataPath = " << module.getConfigEntry("AbsoluteDataPath") << "\n";
        std::cout << "Has Feature HebrewDef = " << module.getConfig().has("Feature", "HebrewDef") << "\n";
        if ((module.getType() == "Biblical Texts") || (module.getType() == "Commentaries")) {
            module.setKey("James 1:19");
            std::cout << module.renderText() << "\n\n";
        }
    }

    if (auto const mhc = mymgr.getModule("MHC")) {
        std::cout << "MHC, Lang = " << mhc->getLanguage() << "\n\n";
        for (mhc->setKey("Gen 1:1"); *mhc->getKey() < (VerseKey) "Gen 1:10"; (*mhc).increment())
            std::cout << mhc->renderText() << "\n";
    }
}
