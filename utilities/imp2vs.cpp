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
  mod->setKey(*vkey);
  
  while (readline(infile, linebuffer)) {
    if (!strncmp(linebuffer, "$$$", 3)) {
      if (strlen(keybuffer) && strlen(entbuffer)) {
	std::cout << keybuffer << std::endl;
	*vkey = keybuffer;
	if (!vkey->Chapter()) {
	  // bad hack:  0:0 is Book intro; (chapter):0 is Chapter intro; 0:2 is Module intro; 0:1 is Testament intro
	  int backstep = vkey->Verse();
	  vkey->Verse(1);
	  vkey->Chapter(1);
	  switch (backstep) {
	  case 2:
		  vkey->Book(1);
		  vkey->Testament(0);
       case 1:
            vkey->Book(0);
		  vkey->Chapter(0);
	  }
       vkey->Verse(0);
	  
	  mod->setEntry(entbuffer, strlen(entbuffer));
	}
	else {
	  ListKey listkey = vkey->ParseVerseList(keybuffer, "Gen1:1", true);
	  int i;
	  bool havefirst = false;
	  VerseKey firstverse;
	  firstverse.Headings(1);
	  firstverse.AutoNormalize(0);
	  for (i = 0; i < listkey.Count(); i++) {
	    VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.GetElement(i));
	    if (element) {
	      *vkey = element->LowerBound();
	      VerseKey finalkey = element->UpperBound();
		  finalkey.Headings(1);
		  finalkey.AutoNormalize(0);
	      std::cout << (const char*)*vkey << "-" << (const char*)finalkey << std::endl;
	      if (!havefirst) {
		havefirst = true;
		firstverse = *vkey;
		
		mod->setEntry(entbuffer, strlen(entbuffer));
		std::cout << "f" << (const char*)firstverse << std::endl;
		(*vkey)++;
	      }
	      while (*vkey <= finalkey) {
		std::cout << (const char*)(*vkey) << std::endl;
		*mod << &firstverse;
		(*vkey)++;
	      }
	    }
	    else {
	      if (havefirst) {
		*vkey = (*listkey.GetElement(i));
		*mod << &firstverse;
		std::cout << (const char*)*vkey << std::endl;
	      }
	      else {
		*vkey = (*listkey.GetElement(i));
		havefirst = true;
		firstverse = *vkey;
		
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
    if (!vkey->Chapter()) {
      // bad hack:  0:0 is Book intro; (chapter):0 is Chapter intro; 0:2 is Module intro; 0:1 is Testament intro
      int backstep = vkey->Verse();
	  vkey->Verse(1);
	  vkey->Chapter(1);
	  switch (backstep) {
	  case 2:
		  vkey->Book(1);
		  vkey->Testament(0);
       case 1:
            vkey->Book(0);
		  vkey->Chapter(0);
	  }
      vkey->Verse(0);
      
      mod->setEntry(entbuffer, strlen(entbuffer));
    }
    else {
      ListKey listkey = vkey->ParseVerseList(keybuffer, "Gen1:1", true);
      int i;
      bool havefirst = false;
      VerseKey firstverse;
	  firstverse.Headings(1);
	  firstverse.AutoNormalize(0);
      for (i = 0; i < listkey.Count(); i++) {
	VerseKey *element = SWDYNAMIC_CAST(VerseKey, listkey.GetElement(i));
	if (element) {
	  *vkey = element->LowerBound();
	  VerseKey finalkey = element->UpperBound();
	  finalkey.Headings(1);
	  finalkey.AutoNormalize(0);
	  std::cout << (const char*)*vkey << "-" << (const char*)finalkey << std::endl;
	  if (!havefirst) {
	    havefirst = true;
	    firstverse = *vkey;
	    
	    ((SWModule*)mod)->setEntry(entbuffer, strlen(entbuffer));
	    std::cout << "f" << (const char*)firstverse << std::endl;
	    (*vkey)++;
	  }
	  while (*vkey <= finalkey) {
	    std::cout << (const char*)*vkey << std::endl;
	    *(SWModule*)mod << &firstverse;
	    (*vkey)++;
	  }
	}
	else {
	  if (havefirst) {
	    *vkey = (*listkey.GetElement(i));
	    *mod << &firstverse;
	    std::cout << (const char*)*vkey << std::endl;
	  }
	  else {
	    *vkey = (*listkey.GetElement(i));
	    havefirst = true;
	    firstverse = *vkey;
	    
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
