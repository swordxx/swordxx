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

#include <entriesblk.h>
#include <iostream>
#include <treekeyidx.h>
#include <rawgenbook.h>

#ifndef NO_SWORD_NAMESPACE
using sword::TreeKeyIdx;
using sword::RawGenBook;
using sword::SWKey;
#endif

void printTree(TreeKeyIdx treeKey, TreeKeyIdx *target = 0, int level = 1) {
  if (!target)
    target = &treeKey;
  
  unsigned long currentOffset = target->getOffset();
  std::cout << ((currentOffset == treeKey.getOffset()) ? "==>" : "");
  for (int i = 0; i < level; i++) std::cout << "\t";
  std::cout << treeKey.getLocalName() << "/\n";
  if (treeKey.firstChild()) {
    printTree(treeKey, target, level+1);
    treeKey.parent();
  }
  if (treeKey.nextSibling())
    printTree(treeKey, target, level);
}

void setKey(TreeKeyIdx * treeKey, char* keyBuffer) {
  char* tok = strtok(keyBuffer, "/");
  while (tok) {
    bool foundkey = false;
    if (treeKey->hasChildren()) {
      treeKey->firstChild();
      if (!strcmp(treeKey->getLocalName(), tok)) {
	foundkey = true;
      } else {
	while (treeKey->nextSibling()) {
	  if (treeKey->getLocalName()) {
	    if (!strcmp(treeKey->getLocalName(), tok)) {
	      foundkey = true;
	    }
	  }
	}
      }
      if (!foundkey) {
	treeKey->append();
	treeKey->setLocalName(tok);
	treeKey->save();	    
      }
    }
    else {
      treeKey->appendChild();
      treeKey->setLocalName(tok);
      treeKey->save();
    }
    
    //DEBUG      std::cout << treeKey->getLocalName() << " : " << tok << std::endl;
    
    tok = strtok(NULL, "/");
    
  }
}

int readline(FILE* infile, char* lineBuffer) {
  signed char c;
  char* lbPtr = lineBuffer;
  while ((c = fgetc(infile)) != EOF) {
    *lbPtr++ = c;
    if (c == 10) {
      *lbPtr = 0;
      return (strlen(lineBuffer));
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  
  const char * helptext ="imp2gbs 1.0 General Book module creation tool for the SWORD Project\n  usage:\n   %s <filename> [modname]\n";
  
  signed long i = 0;
  char* keyBuffer = new char[2048];
  char* entBuffer = new char[1048576];
  char* lineBuffer = new char[1048576];
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
  
  TreeKeyIdx * treeKey;
  RawGenBook *book;
  
  // Do some initialization stuff
  TreeKeyIdx::create(modname);
  treeKey = new TreeKeyIdx(modname);
  RawGenBook::createModule(modname);
  delete treeKey;
  book = new RawGenBook(modname);
  //DEBUG  TreeKeyIdx root = *((TreeKeyIdx *)((SWKey *)(*book)));
  treeKey = ((TreeKeyIdx *)((SWKey *)(*book)));
  
  while (readline(infile, lineBuffer)) {
    if (!strncmp(lineBuffer, "$$$", 3)) {
      if (strlen(keyBuffer) && strlen(entBuffer)) {
	std::cout << keyBuffer << std::endl;
	treeKey->root();
	setKey(treeKey, keyBuffer);
	book->setEntry(entBuffer, strlen(entBuffer));
      }
      lineBuffer[strlen(lineBuffer) - 1] = 0;
      strcpy (keyBuffer, lineBuffer + 3);
      *entBuffer = 0;
    }
    else {
      strcat (entBuffer, lineBuffer);
    }
  }

  //handle final entry
  if (strlen(keyBuffer) && strlen(entBuffer)) {
    std::cout << keyBuffer << std::endl;
    treeKey->root();
    setKey(treeKey, keyBuffer);
    book->setEntry(entBuffer, strlen(entBuffer));
  }
  
  //DEBUG  printTree(root, treeKey);
  
  delete book;
  delete [] keyBuffer;
  delete [] lineBuffer;
  delete [] entBuffer;

  return 0;
}



