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
#include <rawtext.h>
#include <iostream.h>


#ifndef O_BINARY
#define O_BINARY 0
#endif

int main(int argc, char **argv) {
  
  const char * version = "1.0";

  if (argc < 3) {
    fprintf(stderr, "addvs %s module creation tool for the SWORD Project\nUse -a to add a new verse from standard input, -l to link two verses, -c to\ncreate a new module.\n  usage:\n   %s -a </path/to/module> <verse> [</path/to/file/with/verse>]\n   %s -l </path/to/module> <first verse (already assigned)> <second verse>\n   %s -c </path/to/module>\n", version, argv[0], argv[0], argv[0]);
    exit(-1);
  }
 if (!strcmp(argv[1], "-a") && (argc == 4 || argc == 5)) {	
    
    // Do some initialization stuff
    char buffer[65536];  //this is the max size of any entry
    RawText mod(argv[2]);	// open our datapath with our RawText driver.
    VerseKey *vkey = new VerseKey;
    vkey->Headings(1);
    vkey->AutoNormalize(0);
    vkey->Persist(1);      // the magical setting
    
    // Set our VerseKey
    mod.SetKey(*vkey);
    *vkey = argv[3];
    // bad hack >>
    // 0:0 is Book intro
    // (chapter):0 is Chapter intro
    //
    // 0:2 is Module intro
    // 0:1 is Testament intro
    int backstep = 0;
    if (!vkey->Chapter())
      backstep = vkey->Verse();
    // << bad hack
    
    // Make sure we stay in the right book/chapter by setting to vs. 1
    // and (if we're doing a module/testament heading) ch. 1
    if (backstep) {
      vkey->Verse(0);
    }
    mod -= backstep;
    
    FILE *infile;
    // case: add from text file
    //Open our data file and read its contents into the buffer
    if (argc == 5) infile = fopen(argv[4], "r");
    // -case: add from stdin
    else infile = stdin;

    fread(buffer, sizeof(buffer), sizeof(char), infile);
    
    mod << buffer;	// save text to module at current position

    delete vkey;
  }
  // Link 2 verses
  else if (!strcmp(argv[1], "-l") && argc == 5) {
    // Do some initialization stuff
    RawText *mod = new RawText(argv[2]);	// open our datapath with our RawText driver.
    
    mod->SetKey(argv[4]);    // set key from argument
    *(SWModule*)mod << &((SWKey) argv[3]);
    delete mod;
  }

  // Make a new module
  else if (!strcmp(argv[1], "-c") && argc == 3) {
    // Try to initialize a default set of datafiles and indicies at our
    // datapath location passed to us from the user.
    if (RawText::createModule(argv[2])) {
      fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
      exit(-2);
    }
    /*  Module heading bug is not in writing, it's in reading, so -a/-t implementation works fine
    if (argc == 4) {
      int length = 0;
      char buffer[65536];
      FILE* infile = fopen(argv[3], "r");
      char * outfilename = new char;
      strcpy(outfilename, argv[2]);
      strcat(outfilename, "ot");
      FILE* outfile = fopen(outfilename, "w");

      buffer[0] = 0;
      while (!feof(infile)) {
	fputs (buffer, outfile);
	length += strlen(buffer);
	fgets (buffer, sizeof(buffer), infile);
      }
      fclose(outfile);
      strcat(outfilename, ".vss");
      outfile = fopen(outfilename, "rb+");
      fseek(outfile, 4, SEEK_SET);
      fwrite(&length, 2, 1, outfile);
      fclose(outfile);
    }
    */
  }   
  
  // Bad arguments, print usage
  else {
    fprintf(stderr, "addvs %s module creation tool for the SWORD Project\nUse -a to add a new verse from standard input, -t to add a verse from a text\nfile, -l to link two verses, -c to create a new module.\nusage:\n   %s -a </path/to/module> <verse>\n   %s -t </path/to/module> <verse> </path/to/file/with/verse>\n   %s -l </path/to/module> <first verse (already assigned)> <second verse>\n   %s -c </path/to/module>\n", version, argv[0], argv[0], argv[0], argv[0]);
    exit(-1);
  }
}
