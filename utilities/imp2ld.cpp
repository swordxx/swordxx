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
#include <rawld.h>
#include <rawld4.h>
#include <zld.h>
#include <zipcomprs.h>

#ifndef NO_SWORD_NAMESPACE
using sword::zLD;
using sword::ZipCompress;
using sword::RawLD4;
using sword::RawLD;
using sword::SWKey;
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

  const char * helptext ="imp2ld 1.0 Lexicon/Dictionary/Daily Devotional/Glossary module creation tool for the SWORD Project\n  usage:\n   %s <filename> [modname] [ 4 (default) | 2 | z - module driver]\n";

  signed long i = 0;
  char* keybuffer = new char[2048];
  char* entbuffer = new char[1048576];
  char* linebuffer = new char[1048576];
  char modname[16];
  char links = 0;
  char* linkbuffer[32];
  while (i < 32) {
        linkbuffer[i] = new char[256];
        i++;
  }
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
	std::cout << keybuffer << std::endl;
	*key = keybuffer;

	if (mode == 3) {
	  modZ->setKey(*key);
	  modZ->setEntry(entbuffer, strlen(entbuffer));
          for (i = 0; i < links; i++) {
                SWKey tmpkey = linkbuffer[i];
                modZ->linkEntry(&tmpkey);
                std::cout << "Linking: " << linkbuffer[i] << std::endl;
          }
	}
	else if (mode == 2) {
	  mod2->setKey(*key);
	  mod2->setEntry(entbuffer, strlen(entbuffer));
          for (i = 0; i < links; i++) {
                SWKey tmpkey = linkbuffer[i];
                mod2->linkEntry(&tmpkey);
                std::cout << "Linking: " << linkbuffer[i] << std::endl;
          }
	}
	else if (mode == 1) {
	  mod4->setKey(*key);
	  mod4->setEntry(entbuffer, strlen(entbuffer));
          for (i = 0; i < links; i++) {
                SWKey tmpkey = linkbuffer[i];
                mod4->linkEntry(&tmpkey);
                std::cout << "Linking: " << linkbuffer[i] << std::endl;
          }
	}
      }
      linebuffer[strlen(linebuffer) - 1] = 0;
      strcpy (keybuffer, linebuffer + 3);
      *entbuffer = 0;
      links = 0;
    }
    else if (!strncmp(linebuffer, "%%%", 3)) {
      strcpy (linkbuffer[links], linebuffer + 3);
      linkbuffer[links][strlen(linkbuffer[links]) - 1] = 0;
      links++;
    }
    else {
      strcat (entbuffer, linebuffer);
    }
  }

  //handle final entry
  if (strlen(keybuffer) && strlen(entbuffer)) {
    std::cout << keybuffer << std::endl;
    *key = keybuffer;

    if (mode == 3) {
	  modZ->setKey(*key);
          modZ->setEntry(entbuffer, strlen(entbuffer));
          for (i = 0; i < links; i++) {
                SWKey tmpkey = linkbuffer[i];
                modZ->linkEntry(&tmpkey);
                std::cout << "Linking: " << linkbuffer[i] << std::endl;
          }
    }
    else if (mode == 2) {
	  mod2->setKey(*key);
          mod2->setEntry(entbuffer, strlen(entbuffer));
          for (i = 0; i < links; i++) {
                SWKey tmpkey = linkbuffer[i];
                mod2->linkEntry(&tmpkey);
                std::cout << "Linking: " << linkbuffer[i] << std::endl;
          }

    }
    else if (mode == 1) {
          mod4->setKey(*key);
          mod4->setEntry(entbuffer, strlen(entbuffer));
          for (i = 0; i < links; i++) {
                SWKey tmpkey = linkbuffer[i];
                mod4->linkEntry(&tmpkey);
                std::cout << "Linking: " << linkbuffer[i] << std::endl;
          }

    }
  }

  //DEBUG  printTree(root, treeKey);

  delete keybuffer;
  delete entbuffer;
  delete linebuffer;

  return 0;
}
