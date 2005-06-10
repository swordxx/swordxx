#include <iostream>
#include <string>
#include <fstream>
#include <treekeyidx.h>
#include <rawgenbook.h>

using std::string;

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
  std::cout << treeKey.getLocalName() << std::endl;
  if (treeKey.firstChild()) {
    printTree(treeKey, target, level+1);
    treeKey.parent();
  }
  if (treeKey.nextSibling())
    printTree(treeKey, target, level);
}

void setkey (TreeKeyIdx * treeKey, const char* keybuffer) {
  char *keybuf = strdup(keybuffer);
  char* tok = strtok(keybuf, "/");
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
  free(keybuf);
}


int main(int argc, char **argv) {
  
  const char * helptext ="imp2gbs 1.0 General Book module creation tool for the SWORD Project\n  usage:\n   %s <filename> [modname]\n";
  
  signed long i = 0;
  string keybuffer;
  string entbuffer;
  string linebuffer;
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
  
  std::ifstream infile(argv[1]);
  
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
  
  while (!infile.eof()) {
    std::getline(infile, linebuffer);
    if (linebuffer.substr(0,3) == "$$$") {
      if (keybuffer.size() && entbuffer.size()) {
	std::cout << keybuffer << std::endl;
	treeKey->root();
	setkey(treeKey, keybuffer.c_str());
	book->setEntry(entbuffer.c_str(), entbuffer.size());
      }
      keybuffer = linebuffer.substr(3,linebuffer.size()) ;
      entbuffer.resize(0);
    }
    else {
      entbuffer += linebuffer;
    }
  }

  //handle final entry
  if (keybuffer.size() && entbuffer.size()) {
    std::cout << keybuffer << std::endl;
    treeKey->root();
    setkey(treeKey, keybuffer.c_str());
    book->setEntry(entbuffer.c_str(), entbuffer.size());
  }
  
  //DEBUG  printTree(root, treeKey);
  
  delete book;
  infile.close();

  return 0;
}



