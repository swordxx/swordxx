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

#include <iostream>
#include <string>
#include <rawtext.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

int readline(FILE* infile, char* linebuffer) {
  signed char c;
  char* lbPtr = linebuffer;
  while ((c = fgetc(infile)) != EOF) {
    *lbPtr++ = c;
    if (c == 10) {
      *lbPtr = 0;
      return (strlen(linebuffer));
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  
  const char * helptext ="imp2vs 1.0 Bible/Commentary module creation tool for the SWORD Project\n  usage:\n   %s <filename> [output dir] \n";
  
  signed long i = 0;
  char* keybuffer = new char[2048];
  char* entbuffer = new char[1048576];
  char* linebuffer = new char[1048576];
  char modname[16];
  
  if (argc > 2) {
    strcpy (modname, argv[2]);
  }
  else if (argc > 1) {
    strcpy (modname, "./");
  }
  else {
    fprintf(stderr, helptext, argv[0]);
    exit(-1);
  }
  
  FILE *infile;
  infile = fopen(argv[1], "r");

  SWText* mod;
  RawText::createModule(modname);
  RawText modRaw(modname);
  mod = &modRaw;
  
  VerseKey* vkey = new VerseKey;
  vkey->Headings(1);
  vkey->AutoNormalize(0);
  vkey->Persist(1);
  
  while (readline(infile, linebuffer)) {
    if (!strncmp(linebuffer, "$$$", 3)) {
      if (strlen(keybuffer) && strlen(entbuffer)) {
	std::cout << keybuffer << std::endl;
	*vkey = keybuffer;
	mod->setKey(*vkey);
	if (!vkey->Chapter()) {
	  // bad hack:  0:0 is Book intro; (chapter):0 is Chapter intro; 0:2 is Module intro; 0:1 is Testament intro
	  int backstep = vkey->Verse();
	  vkey->Verse(0);
	  *mod -= backstep;       
	  
	  mod->setEntry(entbuffer, strlen(entbuffer));
	}
	else {
	  ListKey listkey = vkey->ParseVerseList(keybuffer, "Gen1:1", true);
	  int i;
	  bool havefirst = false;
	  VerseKey firstverse;
	  for (i = 0; i < listkey.Count(); i++) {
	    VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.GetElement(i));
	    if (element) {
	      mod->Key(element->LowerBound());
	      VerseKey finalkey = element->UpperBound();
	      std::cout << (const char*)mod->Key() << "-" << (const char*)finalkey << std::endl;
	      if (!havefirst) {
		havefirst = true;
		firstverse = mod->Key();
		
		mod->setEntry(entbuffer, strlen(entbuffer));
		std::cout << "f" << (const char*)firstverse << std::endl;
		(mod->Key())++;
	      }
	      while (mod->Key() <= finalkey) {
		std::cout << (const char*)mod->Key() << std::endl;
		*(SWModule*)mod << &firstverse;
		(mod->Key())++;
	      }
	    }
	    else {
	      if (havefirst) {
		mod->Key(*listkey.GetElement(i));
		*(SWModule*)mod << &firstverse;
		std::cout << (const char*)mod->Key() << std::endl;
	      }
	      else {
		mod->Key(*listkey.GetElement(i));
		havefirst = true;
		firstverse = mod->Key();
		
		mod->setEntry(entbuffer, strlen(entbuffer));
		std::cout << "f" << (const char*)firstverse << std::endl;
	      }
	    }
	  }
	}
      }
      linebuffer[strlen(linebuffer) - 1] = 0;
      strcpy (keybuffer, linebuffer + 3);
      *entbuffer = 0;
    }
    else {
      strcat (entbuffer, linebuffer);
    }
  }

  //handle final entry
  if (strlen(keybuffer) && strlen(entbuffer)) {
    std::cout << keybuffer << std::endl;
    *vkey = keybuffer;
    mod->setKey(*vkey);
    if (!vkey->Chapter()) {
      // bad hack:  0:0 is Book intro; (chapter):0 is Chapter intro; 0:2 is Module intro; 0:1 is Testament intro
      int backstep = vkey->Verse();
      vkey->Verse(0);
      *mod -= backstep;       
      
      mod->setEntry(entbuffer, strlen(entbuffer));
    }
    else {
      ListKey listkey = vkey->ParseVerseList(keybuffer, "Gen1:1", true);
      int i;
      bool havefirst = false;
      VerseKey firstverse;
      for (i = 0; i < listkey.Count(); i++) {
	VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.GetElement(i));
	if (element) {
	  mod->Key(element->LowerBound());
	  VerseKey finalkey = element->UpperBound();
	  std::cout << (const char*)mod->Key() << "-" << (const char*)finalkey << std::endl;
	  if (!havefirst) {
	    havefirst = true;
	    firstverse = mod->Key();
	    
	    ((SWModule*)mod)->setEntry(entbuffer, strlen(entbuffer));
	    std::cout << "f" << (const char*)firstverse << std::endl;
	    (mod->Key())++;
	  }
	  while (mod->Key() <= finalkey) {
	    std::cout << (const char*)mod->Key() << std::endl;
	    *(SWModule*)mod << &firstverse;
	    (mod->Key())++;
	  }
	}
	else {
	  if (havefirst) {
	    mod->Key(*listkey.GetElement(i));
	    *mod << &firstverse;
	    std::cout << (const char*)mod->Key() << std::endl;
	  }
	  else {
	    mod->Key(*listkey.GetElement(i));
	    havefirst = true;
	    firstverse = mod->Key();
	    
	    mod->setEntry(entbuffer, strlen(entbuffer));
	    std::cout << "f" << (const char*)firstverse << std::endl;
	  }
	}
      }
    }
  }

  //DEBUG  printTree(root, treeKey);

  delete entbuffer;
  delete linebuffer;
  delete keybuffer;

  return 0;
}
