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

#include <iostream.h>
#include <string>
#include <rawld.h>
#include <rawld4.h>
#include <zld.h>
#include <zipcomprs.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

int readline(FILE* infile, char* linebuffer) {
  char c;
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
  
  const char * helptext ="imp2ld 1.0 Lexicon/Dictionary/Daily Devotional/Glossary module creation tool for the SWORD Project\n  usage:\n   %s <filename> [modname] [ 4 (default) | 2 | z - module driver]\n";
  
  signed long i = 0;
  char keybuffer[2048];
  char entbuffer[1048576];
  char linebuffer[1048576];
  char modname[16];
  
  if (argc > 2) {
    strcpy (modname, argv[2]);
  }
  else if (argc > 1) {
    for (i = 0; (i < 16) && (argv[1][i]) && (argv[1][i] != '.'); i++) {
      modname[i] = argv[1][i];
    }
    modname[i] = 0;
  }
  else {
    fprintf(stderr, helptext, argv[0]);
    exit(-1);
  }
  
  FILE *infile;
  infile = fopen(argv[1], "r");

  char mode = 1;
  if (argc > 3) {
    switch (*argv[3]) {
    case 'z':
      mode = 3;
      break;
    case '2':
      mode = 2;
      break;
    default:
      mode = 1;
    }
  }
  
  zLD* modZ;
  RawLD* mod2;
  RawLD4* mod4;
  SWKey* key;

  if (mode == 3) {
    zLD::createModule(modname);
    modZ = new zLD(modname, 0, 0, 30, new ZipCompress());
    key = modZ->CreateKey();
  }
  else if (mode == 2) {
    RawLD::createModule(modname);
    mod2 = new RawLD(modname);
    key = mod2->CreateKey();
  }
  else if (mode == 1) {
    RawLD4::createModule(modname);
    mod4 = new RawLD4(modname);
    key = mod4->CreateKey();
  }
  
  key->Persist(1);
  
  while (readline(infile, linebuffer)) {
    if (!strncmp(linebuffer, "$$$", 3)) {
      if (strlen(keybuffer) && strlen(entbuffer)) {
	cout << keybuffer << endl;
	*key = keybuffer;

	if (mode == 3) {
	  modZ->SetKey(*key);
	  modZ->setentry(entbuffer, strlen(entbuffer));
	}
	else if (mode == 2) {
	  mod2->SetKey(*key);
	  mod2->setentry(entbuffer, strlen(entbuffer));
	}
	else if (mode == 1) {
	  mod4->SetKey(*key);
	  mod4->setentry(entbuffer, strlen(entbuffer));
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
    cout << keybuffer << endl;
    *key = keybuffer;

    if (mode == 3) {
      modZ->SetKey(*key);
      modZ->setentry(entbuffer, strlen(entbuffer));
    }
    else if (mode == 2) {
      mod2->SetKey(*key);
      mod2->setentry(entbuffer, strlen(entbuffer));
    }
    else if (mode == 1) {
      mod4->SetKey(*key);
      mod4->setentry(entbuffer, strlen(entbuffer));
    }
  }
  
  //DEBUG  printTree(root, treeKey);
  
  return 0;
}
