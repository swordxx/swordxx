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
#include <string>
#include <stdio.h>
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


int main(int argc, char **argv) {
  
  const char * helptext ="addgb 1.0 General Book module creation tool for the SWORD Project\nUse -a to add a new leaf entry from standard input or a file\n  usage:\n   %s -a <filename> <key> [</path/to/file/with/entry>]\n";

  //  const char * helptext ="addgb 1.0 General Book module creation tool for the SWORD Project\nUse -a to add a new leaf entry from standard input or a file, -d to delete an\nentry, -l to link two leaf entries.\n  usage:\n   %s -a <filename> <key> [</path/to/file/with/entry>]\n   %s -d <filename> <key>\n   %s -l <filename> <first key (already assigned)> <second key>\n";

  char mode;
  unsigned long entrysize;
    
  if (argc < 3) {
    fprintf(stderr, helptext, argv[0]);
    exit(-1);
  }
 
  mode = argv[1][1];

  // Do some initialization stuff
  TreeKeyIdx *treeKey = new TreeKeyIdx(argv[2]);
  if (treeKey->Error()) {
    treeKey->create(argv[2]);
    delete treeKey;
    treeKey = new TreeKeyIdx(argv[2]);
    RawGenBook::createModule(argv[2]);
  }
  delete treeKey;
  RawGenBook *book = new RawGenBook(argv[2]);
  TreeKeyIdx root = *((TreeKeyIdx *)((SWKey *)(*book)));
  treeKey = (TreeKeyIdx *)(SWKey *)(*book);  

  if ((mode == 'a') && (argc == 4 || argc == 5)) {	
    char buffer[1048576];  //this is the max size of any entry

    char* tok = strtok(argv[3], "/");
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
      
      //DEBUG      std::cout << treeKey->getLocalName() << " : " << tok << endl;
      
      tok = strtok(NULL, "/");
      
    }
    
    FILE *infile;
    // case: add from text file
    //Open our data file and read its contents into the buffer
    if (argc == 5) infile = fopen(argv[4], "r");
    // case: add from stdin
    else infile = stdin;
    
    entrysize = fread(buffer, sizeof(char), sizeof(buffer), infile);
    book->setEntry(buffer, entrysize); // save text to module at current position
  }
  
  /*
    // let's pretend these don't exist for the time being

    // Link 2 verses
    else if ((mode == 'l') && argc == 5) {
    *key = argv[3];
    mod.setKey(*key);
    
    SWKey tmpkey = argv[4];
    mod << &(tmpkey);
    }
    
    // Delete an entry
    else if ((mode == 'd') && argc == 4) {
    mod.setKey(argv[3]);
    mod.deleteEntry();
    }
  */
  // Bad arguments, print usage
  else {
    fprintf(stderr, helptext, argv[0]);
    exit(-1);
  }
  
  //DEBUG  printTree(root, treeKey);
  
  delete treeKey;
  delete book;
  return 0;
}
