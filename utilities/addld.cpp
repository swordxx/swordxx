#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <swmgr.h>
#include <rawld.h>
#include <iostream.h>


#ifndef O_BINARY
#define O_BINARY 0
#endif

int main(int argc, char **argv) {
  
  const char * helptext ="addld 1.0 Lexicon & Dictionary module creation tool for the SWORD Project\nUse -a to add a new LD entry from standard input or a file, -d to delete an\nentry, -l to link two LD entries, -c to create a new module.\n  usage:\n   %s -a <filename> <key> [</path/to/file/with/entry>]\n   %s -d <filename> <key>\n   %s -l <filename> <first key (already assigned)> <second key>\n   %s -c <filename>\n";

  if (argc < 3) {
    fprintf(stderr, helptext, argv[0], argv[0], argv[0], argv[0]);
    exit(-1);
  }
 if (!strcmp(argv[1], "-a") && (argc == 4 || argc == 5)) {	
    
    // Do some initialization stuff
    char buffer[65536];  //this is the max size of any entry
    RawLD mod(argv[2]);	// open our datapath with our RawText driver.
    SWKey* key = mod.CreateKey();
    key->Persist(1);      // the magical setting

    // Set our VerseKey
    *key = argv[3];
    mod.SetKey(*key);
    FILE *infile;
    // case: add from text file
    //Open our data file and read its contents into the buffer
    if (argc == 5) infile = fopen(argv[4], "r");
    // case: add from stdin
    else infile = stdin;

    fread(buffer, sizeof(buffer), sizeof(char), infile);
    mod << buffer;	// save text to module at current position

  }
  // Link 2 verses
 else if (!strcmp(argv[1], "-l") && argc == 5) {
   // Do some initialization stuff
   RawLD mod(argv[2]);	// open our datapath with our RawText driver.
   SWKey* key = mod.CreateKey();
   key->Persist(1);      // the magical setting
   
   *key = argv[3];
   mod.SetKey(*key);
   
   mod << &((SWKey) argv[4]);
 }
 else if (!strcmp(argv[1], "-d") && argc == 4) {
   RawLD mod(argv[2]);	// open our datapath with our RawText driver.
   mod.SetKey(argv[3]);
   mod.deleteEntry();
 }
  // Make a new module
  else if (!strcmp(argv[1], "-c") && argc == 3) {
    // Try to initialize a default set of datafiles and indicies at our
    // datapath location passed to us from the user.
    if (RawLD::createModule(argv[2])) {
      fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
      exit(-2);
    }
  }   
  
  // Bad arguments, print usage
  else {
    fprintf(stderr, helptext, argv[0], argv[0], argv[0], argv[0]);
    exit(-1);
  }
}
