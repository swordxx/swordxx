/******************************************************************************
 *
 *  addvs.cpp -    Utility to create/modify a VerseKey module by adding a single
 *        entry
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
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#ifndef __GNUC__
#include <io.h>
#endif
#include <swordxx/keys/versekey.h>
#include <swordxx/modules/texts/rawtext.h>
#include <swordxx/swmgr.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using swordxx::SWMgr;
using swordxx::RawText;
using swordxx::SWKey;
using swordxx::VerseKey;
using swordxx::ListKey;
using swordxx::SWModule;

int main(int argc, char **argv) {

  const char * helptext = "addvs 1.1 Bible & Commentary module creation tool for the Sword++ Project\nUse -a to add a new verse from standard input or a file, -d to delete a verse,\n-l to link two verses, -c to create a new module.\n  usage:\n   %s -a </path/to/module> <verse> [</path/to/file/with/verse>]\n   %s -d </path/to/module> <key>\n   %s -l </path/to/module> <first verse (already assigned)> <second verse>\n   %s -c </path/to/module>\n";
  long entrysize;

  if (argc < 3) {
    fprintf(stderr, helptext, argv[0], argv[0], argv[0], argv[0]);
    std::exit(-1);
  }
 if (!std::strcmp(argv[1], "-a") && (argc == 4 || argc == 5)) {

    // Do some initialization stuff
    char buffer[65536];  //this is the max size of any entry
    RawText mod(argv[2]);    // open our datapath with our RawText driver.
    auto vkey(std::make_shared<VerseKey>());
    vkey->setIntros(true);
    vkey->setAutoNormalize(false);
    vkey->setText(argv[3]);
    // Set our VerseKey
    mod.setKey(vkey);
    if (!vkey->getChapter()) {
      // bad hack >>
      // 0:0 is Book intro
      // (chapter):0 is Chapter intro
      //
      // 0:2 is Module intro
      // 0:1 is Testament intro
      int backstep = vkey->getVerse();
      vkey->setVerse(0);
      mod.decrement(backstep);
      // << bad hack

      FILE *infile;
      // case: add from text file
      //Open our data file and read its contents into the buffer
      if (argc == 5) infile = fopen(argv[4], "r");
      // case: add from stdin
      else infile = stdin;

      entrysize = fread(buffer, sizeof(char), sizeof(buffer), infile);

      mod.setEntry(std::string_view(buffer, entrysize));    // save text to module at current position
    }
    else {
      ListKey listkey = vkey->parseVerseList(argv[3], "Gen1:1", true);
      std::size_t i;
      bool havefirst = false;
      VerseKey firstverse;
      for (i = 0u; i < listkey.getCount(); ++i) {
    if (auto const element =
        std::dynamic_pointer_cast<VerseKey const>(listkey.getElement(i)))
    {
      mod.setKey(element->lowerBoundKey().clone());
      VerseKey finalkey = element->upperBoundKey();
      std::cout << mod.getKeyText() << "-" << finalkey.getText() << std::endl;
      if (!havefirst) {
        havefirst = true;
        firstverse.positionFrom(*mod.getKey());
        FILE *infile;
        // case: add from text file
        //Open our data file and read its contents into the buffer
        if (argc == 5) infile = fopen(argv[4], "r");
        // case: add from stdin
        else infile = stdin;

        entrysize = fread(buffer, sizeof(char), sizeof(buffer), infile);

        mod.setEntry(std::string_view(buffer, entrysize));    // save text to module at current position
        std::cout << "f" << firstverse.getText() << std::endl;
        mod.increment();
      }
      while (*mod.getKey() <= finalkey) {
        std::cout << mod.getKeyText() << std::endl;
        mod.linkEntry(firstverse);
        mod.increment();
      }
    }
    else {
      if (havefirst) {
        mod.setKey(listkey.getElement(i)->clone());
        mod.linkEntry(firstverse);
        std::cout << mod.getKeyText() << std::endl;
      }
      else {
        mod.setKey(listkey.getElement(i)->clone());
        havefirst = true;
        firstverse.positionFrom(*mod.getKey());
        FILE *infile;
        // case: add from text file
        //Open our data file and read its contents into the buffer
        if (argc == 5) infile = fopen(argv[4], "r");
        // case: add from stdin
        else infile = stdin;

        entrysize = fread(buffer, sizeof(char), sizeof(buffer), infile);

        mod.setEntry(std::string_view(buffer, entrysize));    // save text to module at current position
        std::cout << "f" << firstverse.getText() << std::endl;
      }
    }
      }
    }
 }
 // Link 2 verses
 else if (!std::strcmp(argv[1], "-l") && argc == 5) {
   // Do some initialization stuff
   RawText mod(argv[2]);    // open our datapath with our RawText driver.

   mod.setKey(argv[4]);    // set key from argument
   SWKey tmpkey = (SWKey) argv[3];
   mod.linkEntry(tmpkey);
 }

 else if (!std::strcmp(argv[1], "-d") && argc == 4) {
   RawText mod(argv[2]);    // open our datapath with our RawText driver.
   auto vkey(std::make_shared<VerseKey>());
   vkey->setIntros(true);
   vkey->setAutoNormalize(false);

   // Set our VerseKey
   mod.setKey(vkey);
   vkey->setText(argv[3]);

   if (!vkey->getChapter())
     {
       // bad hack >>
       // 0:0 is Book intro
       // (chapter):0 is Chapter intro
       //
       // 0:2 is Module intro
       // 0:1 is Testament intro
       int backstep = vkey->getVerse();
       vkey->setVerse(0);
       mod.decrement(backstep);
       // << bad hack
     }

   mod.deleteEntry();
 }

  // Make a new module
  else if (!std::strcmp(argv[1], "-c") && argc == 3) {
    // Try to initialize a default set of datafiles and indicies at our
    // datapath location passed to us from the user.
    if (RawText::createModule(argv[2])) {
      fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
      std::exit(-2);
    }
  }

  // Bad arguments, print usage
  else {
    fprintf(stderr, helptext, argv[0], argv[0], argv[0], argv[0]);
    std::exit(-1);
  }
}
