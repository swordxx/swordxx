/******************************************************************************
 *
 *  addld.cpp -    Utility to build/modify an LD module by adding a single entry
 *
 * $Id$
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
#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#endif
#include <memory>
#include <swordxx/modules/common/zipcomprs.h>
#include <swordxx/modules/lexdict/rawld.h>
#include <swordxx/modules/lexdict/rawld4.h>
#include <swordxx/modules/lexdict/zld.h>
#include <swordxx/swmgr.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using swordxx::SWMgr;
using swordxx::ZipCompress;
using swordxx::RawLD4;
using swordxx::SWKey;
using swordxx::zLD;
using swordxx::RawLD;


int main(int argc, char **argv) {

  const char * helptext ="addld 1.0 Lexicon & Dictionary module creation tool for the Sword++ Project\nUse -a to add a new LD entry from standard input or a file, -d to delete an\nentry, -l to link two LD entries, -c to create a new module.\n  usage:\n   %s -a <filename> <key> [</path/to/file/with/entry>]\n   %s -d <filename> <key>\n   %s -l <filename> <first key (already assigned)> <second key>\n   %s -c <filename>\nTo use 4-byte LD instead of 2-byte, insert a 4 immediately after the '-'.\nTo use zLD instead of 2-byte, insert a z immediately after the '-'.\n";
  long entrysize;

  bool fourbyte = false;
  bool compress = false;
  char mode;

  if (argc < 3) {
    fprintf(stderr, helptext, argv[0], argv[0], argv[0], argv[0]);
    std::exit(-1);
  }

  if (argv[1][1] == '4') {
    fourbyte = false;
    mode = argv[1][2];
  }
  else if (argv[1][1] == 'z') {
    compress = true;
    mode = argv[1][2];
  }
  else {
    mode = argv[1][1];
  }

  if ((mode == 'a') && (argc == 4 || argc == 5)) {

    // Do some initialization stuff
    if (fourbyte) {
      char buffer[1048576];  //this is the max size of any entry
      RawLD4 mod(argv[2]);    // open our datapath with our RawText driver.
      auto const key(mod.createKey());

      // Set our VerseKey
      key->setText(argv[3]);
      mod.setKey(key);
      FILE *infile;
      // case: add from text file
      //Open our data file and read its contents into the buffer
      if (argc == 5) infile = fopen(argv[4], "r");
      // case: add from stdin
      else infile = stdin;

      entrysize = fread(buffer, sizeof(char), sizeof(buffer), infile);
      mod.setEntry(buffer, entrysize);    // save text to module at current position
    }
    else if (compress) {
      char buffer[1048576];  //this is the max size of any entry
      zLD mod(argv[2], nullptr, nullptr, 200, std::make_unique<ZipCompress>());    // open our datapath with our RawText driver.
      auto const key(mod.createKey());

      // Set our VerseKey
      key->setText(argv[3]);
      mod.setKey(key);
      FILE *infile;
      // case: add from text file
      //Open our data file and read its contents into the buffer
      if (argc == 5) infile = fopen(argv[4], "r");
      // case: add from stdin
      else infile = stdin;

      entrysize = fread(buffer, sizeof(char), sizeof(buffer), infile);
      mod.setEntry(buffer, entrysize);    // save text to module at current position
    }
    else {
      char buffer[65536];  //this is the max size of any entry
      RawLD mod(argv[2]);    // open our datapath with our RawText driver.
      auto const key(mod.createKey());

      // Set our VerseKey
      key->setText(argv[3]);
      mod.setKey(key);
      FILE *infile;
      // case: add from text file
      //Open our data file and read its contents into the buffer
      if (argc == 5) infile = fopen(argv[4], "r");
      // case: add from stdin
      else infile = stdin;

      entrysize = fread(buffer, sizeof(char), sizeof(buffer), infile);
      mod.setEntry(buffer, entrysize);    // save text to module at current position
    }

  }
  // Link 2 verses
  else if ((mode == 'l') && argc == 5) {
    // Do some initialization stuff
    if (fourbyte) {
      RawLD4 mod(argv[2]);    // open our datapath with our RawText driver.
      auto const key(mod.createKey());

      key->setText(argv[3]);
      mod.setKey(key);
      SWKey tmpkey = argv[4];
      mod.linkEntry(tmpkey);
    }
    else if (compress) {
      zLD mod(argv[2]);    // open our datapath with our RawText driver.
      auto const key(mod.createKey());

      key->setText(argv[3]);
      mod.setKey(key);

      SWKey tmpkey = argv[4];
      mod.linkEntry(tmpkey);
    }
    else {
      RawLD mod(argv[2]);    // open our datapath with our RawText driver.
      auto const key(mod.createKey());

      key->setText(argv[3]);
      mod.setKey(key);

      SWKey tmpkey = argv[4];
      mod.linkEntry(tmpkey);
    }
  }
  else if ((mode == 'd') && argc == 4) {
    if (fourbyte) {
      RawLD4 mod(argv[2]);    // open our datapath with our RawText driver.
     mod.setKey(argv[3]);
      mod.deleteEntry();
    }
    if (compress) {
      zLD mod(argv[2]);    // open our datapath with our RawText driver.
     mod.setKey(argv[3]);
      mod.deleteEntry();
    }
    else {
      RawLD mod(argv[2]);    // open our datapath with our RawText driver.
      mod.setKey(argv[3]);
      mod.deleteEntry();
    }

  }
  // Make a new module
  else if ((mode == 'c') && argc == 3) {
    // Try to initialize a default set of datafiles and indicies at our
    // datapath location passed to us from the user.
    if (fourbyte) {
      if (RawLD4::createModule(argv[2])) {
    fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
    std::exit(-2);
      }
    }
    if (compress) {
      if (zLD::createModule(argv[2])) {
    fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
    std::exit(-2);
      }
    }
    else {
      if (RawLD::createModule(argv[2])) {
    fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
    std::exit(-2);
      }
    }
  }

  // Bad arguments, print usage
  else {
    fprintf(stderr, helptext, argv[0], argv[0], argv[0], argv[0]);
    std::exit(-1);
  }
}
