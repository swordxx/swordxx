/******************************************************************************
 *
 *  parallelbibles.cpp -    This examples demonstrates a strategy for
 *                  displaying Bibles in parallel
 *
 * Copyright 2013-2014 CrossWire Bible Society (http://www.crosswire.org)
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
#include <memory>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>
#include <vector>


using namespace swordxx;

const bool o = 1;

void parallelDisplay(std::vector<std::shared_ptr<SWModule> > const & modules,
                     const char * key)
{

    //cout << "Start key:" << key;

    // We'll use the first module's key as our master key to position all other modules.
    std::shared_ptr<VerseKey> master(
                std::static_pointer_cast<VerseKey>(modules[0]->createKey()));

    master->setText(key);

    //cout << "\t key:" << master->getText();

    int curVerse   = master->getVerse();
    int curChapter = master->getChapter();
    int curBook    = master->getBook();

    for (master->setVerse(1);       (master->getBook()    == curBook)
                    && (master->getChapter() == curChapter)
                    && !master->popError();
                                        master->increment()) {

        if (o)
            std::cout << "<tr class=\"" << (master->getVerse() == curVerse ? "currentverse":"verse") << "\">";

        for (auto module = modules.begin(); module != modules.end(); ++module) {
            //cout << "\n\n====================\nfromKey" << master->getOSISRef();

            (*module)->setKey(master);
            VerseKey slave((*module)->getKey()->getText());

            //cout << "setKey" << (*module)->getName() << slave.getBookName() << slave.getRangeText() << slave.getShortText();

            if (o)
                std::cout << "<td>" << "<span class=\"versenum\">";

            //cout << "[" << (int)slave.getBook() << " " << (int)master->getBook() << " " << (int)slave.getTestament() << " " << (int)master->getTestament() << "]";


            if (!(*module)->popError()) {

                if (slave.getBookName() != master->getBookName()) {
                    if (o)
                        std::cout << slave.getShortText();
                }
                else if(slave.getChapter() != master->getChapter()) {
                    if (o)
                        std::cout << slave.getChapter() << ":" << slave.getVerse();
                }
                else {
                    if (o)
                        std::cout << slave.getVerse();
                }

                if(slave.isBoundSet()) {
                    if (o)
                        std::cout << "-";
                    if(slave.upperBoundKey().getBook() != slave.lowerBoundKey().getBook()) {
                        if (o)
                            std::cout << slave.upperBoundKey().getShortText();
                    }
                    else if(slave.upperBoundKey().getChapter() != slave.lowerBoundKey().getChapter()) {
                        if (o)
                            std::cout << slave.upperBoundKey().getChapter() << ":" << slave.upperBoundKey().getVerse();
                    }
                    else {
                        if (o)
                            std::cout << slave.upperBoundKey().getVerse();
                    }
                }

                if (o)
                    std::cout << "</span> ";


                if(slave.isBoundSet()) {
                    auto temp(slave.clone());
                    (*module)->setKey(temp);
                    for(int i = slave.lowerBoundKey().getIndex(); i <= slave.upperBoundKey().getIndex(); ++i) {
                        if (o && (i > 0))
                            std::cout << " ";
                        temp->setIndex(i);
                        if (o)
                            std::cout << (*module)->renderText();
                    }
                }
                else {
                    if (o)
                        std::cout << (*module)->renderText();
                }

                if (o)
                    std::cout << "</td>";
            }
        }
        if (o)
            std::cout << "</tr>";
    }
}


void outputHeader(std::vector<std::shared_ptr<SWModule>> const & modules,
                  char const * key)
{

    modules[0]->setKey(key);

    // force a render so our key snaps to a module entry and we get nicely formatted keytext output later
    // otherwise we just get what the user typed
    modules[0]->renderText();

    std::cout
    << "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">"
    << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:og=\"http://ogp.me/ns#\" xmlns:fb=\"https://www.facebook.com/2008/fbml\" xml:lang=\"en_US\" lang=\"en_US\">"
    << "<head profile=\"http://www.w3.org/2000/08/w3c-synd/#\">"
    << "<meta name=\"keywords\" content=\"Jesus, Christ, Church, Bible, Iran, Persian, Persia, Azeri, Azerbaijan, Armenian, God, Gospel, CrossWire, Java, Web, Software\" />"
    << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
    << "<title>OSIS Web: " << modules[0]->getKeyText() << " - Parallel Bible study</title>"
    << "<link rel=\"stylesheet\" type=\"text/css\" media=\"all\" href=\"http://crosswire.org/study/common.css\"  />"
    << "<link rel=\"stylesheet\" type=\"text/css\" media=\"all\" title=\"Washed Out\" href=\"http://crosswire.org/study/wash.css\"  />"
    << "<link rel=\"stylesheet\" type=\"text/css\" href=\"http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/themes/base/jquery-ui.css\"/>"
    << "<script type=\"text/javascript\" src=\"http://ajax.googleapis.com/ajax/libs/jquery/1.8/jquery.min.js\"></script>"
    << "<script type=\"text/javascript\" src=\"http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/jquery-ui.min.js\"></script>"
    << "<link rel=\"alternate stylesheet\" type=\"text/css\" media=\"all\" title=\"Washed Out\" href=\"http://crosswire.org/study/wash.css\" />"
    << "<link rel=\"alternate stylesheet\" type=\"text/css\" media=\"all\" title=\"Parchment\" href=\"http://crosswire.org/study/parchment.css\" />"
    << "<link rel=\"alternate stylesheet\" type=\"text/css\" media=\"all\" title=\"Sandy Creek\" href=\"http://crosswire.org/study/sandy.css\" />"
    << "<!--For printing stuff -->"
    << "<link rel=\"stylesheet\" type=\"text/css\" media=\"print\" href=\"http://crosswire.org/study/print.css\" />"
    << "<script type=\"text/javascript\" src=\"http://crosswire.org/study/swordweb.js\"></script>"
    << "</head>"
    << "<body onload=\"onPageLoad();\" class=\"ltor\">"
     <<   "<div id=\"pageBorderTop\"></div>"
      <<   "<div id=\"header\">"
        <<     "<h1>The Bible Tool</h1>"
      <<   "</div>"
    <<   "<div id=\"content-main\">"
    <<     "<div id=\"paralleldisplay\">"
    <<       "<style>"
    <<         modules[0]->getRenderHeader()
    <<       "</style>"
    <<       "<h2>Parallel Viewing: " << modules[0]->getKeyText() << "</h2><br/>"
    <<       "<table><thead><tr>";

    for (auto module = modules.begin(); module != modules.end(); ++module) {
        std::cout << "<th>" << (*module)->getDescription() << "</th>";
    }

    std::cout << "</tr></thead><tbody>";


}

void outputFooter(std::vector<std::shared_ptr<SWModule>> const & modules) {

    std::cout << "</tbody><tfoot><tr>";

    for (auto module = modules.begin(); module != modules.end(); ++module) {
        std::string copyLine = (*module)->getConfigEntry("ShortCopyright");
        std::string promoLine = (*module)->getConfigEntry("ShortPromo");
        std::cout
        << "<th>"
        << "<div class=\"copyLine\">" <<  copyLine << "</div>"
        << "<div class=\"promoLine\">" << promoLine << "</div>"
        << "</th>";
    }

    std::cout
    <<       "</tr></tfoot></table>"
    <<     "</div>"
    <<   "</div>"
    <<   "<div id=\"footer\">"
    <<     "<p>"
    <<       "<a href=\"http://www.americanbible.org/\" title=\"American Bible Society\"><img src=\"http://crosswire.org/study/images/logo_abs.gif\" width=\"105\" height=\"64\" alt=\"American Bible Society\" /></a>"
    <<       "<a href=\"http://crosswire.org/\" title=\"CrossWire Bible Society\"><img src=\"http://crosswire.org/study/images/logo_cwbs.gif\" width=\"161\" height=\"64\" alt=\"CrossWire Bible Society\" /></a>"
    <<       "<a href=\"http://sbl-site.org/\" title=\"Society of Biblical Literature\"><img src=\"http://crosswire.org/study/images/logo_fbl.gif\" width=\"86\" height=\"64\" alt=\"Society of Biblical Literature\" /></a>"
    <<     "</p>"
    <<     "<p>Copyright &copy; 2003-2011  <a href=\"http://www.crosswire.org/\">CrossWire Bible Society</a></p>"
    <<       "<div id=\"pageBorderBottom\"><br /></div>"
    <<   "</div>"
    << "</body>"
    << "</html>";
}


int main(int argc, char **argv) {

    SWMgr library(std::make_shared<MarkupFilterMgr>(FMT_XHTML));

    if (argc < 3) {
        fprintf(stderr, "\nusage: %s <modname> [modname ...] <\"key\">\n"
                             "\tExample: %s KJV ESV \"James 1:19\"\n\n", argv[0], argv[0]);
        std::exit(-1);
    }

    std::vector<std::shared_ptr<SWModule>> modules;
    for (int i = 1; i < argc-1; ++i) {
        auto bible(library.getModule(argv[i]));
        if (!bible) {
            fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[i]);
            for (auto const & mp : library.modules())
                fprintf(stderr, "[%s]\t - %s\n",
                        mp.second->getName().c_str(),
                        mp.second->getDescription().c_str());
            std::exit(-2);
        }
        modules.emplace_back(std::move(bible));
    }

    if(o) outputHeader(modules, argv[argc-1]);
    parallelDisplay(modules, argv[argc-1]);
    if(o) outputFooter(modules);

    return 0;
}

