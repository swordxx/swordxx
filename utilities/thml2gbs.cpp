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
#include <iostream.h>
#include <string>
#include <stdio.h>
#include <treekeyidx.h>
#include <rawgenbook.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

void printTree(TreeKeyIdx treeKey, TreeKeyIdx *target = 0, int level = 1) {
	if (!target)
		target = &treeKey;
	
	unsigned long currentOffset = target->getOffset();
       	cout << ((currentOffset == treeKey.getOffset()) ? "==>" : "");
	for (int i = 0; i < level; i++) cout << "\t";
	cout << treeKey.getLocalName() << "/\n";
	if (treeKey.firstChild()) {
		printTree(treeKey, target, level+1);
		treeKey.parent();
	}
	if (treeKey.nextSibling())
		printTree(treeKey, target, level);

}



void setkey (TreeKeyIdx * treeKey, char* keybuffer) {
    char* tok = strtok(keybuffer, "/");
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
      
      //DEBUG      cout << treeKey->getLocalName() << " : " << tok << endl;
      
      tok = strtok(NULL, "/");
      
    }
}

int main(int argc, char **argv) {
  
  const char * helptext ="thml2gbs 1.0 General Book module creation tool for the SWORD Project\n  usage:\n   %s <filename> [modname] [0|1 - long names] [0|1 - import format]\n";

  signed long i = 0;
  string divs[6];
  int level = 0;
  char keybuffer[2048];
  char entbuffer[1048576];
  char modname[16];
  unsigned long entrysize = 0;
  unsigned long keysize = 0;

  bool closer = false;
  bool longnames = false;
  if (argc > 3)
    if (strcmp(argv[3], "0"))
      longnames = true;
  bool exportfile = false;
  if (argc > 4)
    if (strcmp(argv[4], "0"))
      exportfile = true;


  char n[256];
  char type[256];
  char title[512];
  
  char* strtmp;

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
  FILE *outfile;
  if (exportfile) {
    strcat (modname, ".imp");
    outfile = fopen(modname, "w");
  }
  
  TreeKeyIdx * treeKey;
  RawGenBook *book;

  if (!exportfile) {
    // Do some initialization stuff
    TreeKeyIdx::create(modname);
    treeKey = new TreeKeyIdx(modname);
    RawGenBook::createModule(modname);
    delete treeKey;
    book = new RawGenBook(modname);
    //DEBUG  TreeKeyIdx root = *((TreeKeyIdx *)((SWKey *)(*book)));
    treeKey = ((TreeKeyIdx *)((SWKey *)(*book)));
  }
  
  int c;
  while ((c = fgetc(infile)) != EOF) {
    if (c == '<') {
      if (fgets( keybuffer, 6, infile )) {
	if (!strncmp(keybuffer, "/div", 4) && (keybuffer[4] > '0' && keybuffer[4] < '7')) {
	  if (!closer) {
	    keysize = 0;
	    keybuffer[0] = 0;
	    for (i = 0; i < level; i++) {
	      keybuffer[keysize] = '/';
	      keysize++;
	      keybuffer[keysize] = 0;
	      strcat (keybuffer, divs[i].c_str());
	      keysize += divs[i].length();
	    }

	    if (level) {
	      cout << keybuffer << endl;
	      if (exportfile) {
		fprintf (outfile, "$$$%s\n%s\n", keybuffer, entbuffer);
	      }
	      else {
		treeKey->root();
		setkey(treeKey, keybuffer);
		book->setentry(entbuffer, entrysize); // save text to module at current position
	      }
	    }
	  }	    
	  level--;
	  entbuffer[0] = 0;
	  entrysize = 0;
	  
	  closer = true;
	}
	else if (strncmp(keybuffer, "div", 3)
		 || (keybuffer[3] < '1' || keybuffer[3] > '6')) {
	  for (i = 4; i >= 0; i--) {
	    ungetc(keybuffer[i], infile);
	  }
	  entbuffer[entrysize] = '<';
	  entrysize++;
	  entbuffer[entrysize] = 0;
	}
	else {
	  //we have a divN...
	  if (!closer) {
	    keysize = 0;
	    keybuffer[0] = 0;
	    for (i = 0; i < level; i++) {
	      keybuffer[keysize] = '/';
	      keysize++;
	      keybuffer[keysize] = 0;
	      strcat (keybuffer, divs[i].c_str());
	      keysize += divs[i].length();
	    }
	    
	    if (level) {
	      cout << keybuffer << endl;
	      if (exportfile) {
		fprintf (outfile, "$$$%s\n%s\n", keybuffer, entbuffer);
	      }
	      else {
		treeKey->root();
		setkey(treeKey, keybuffer);
		book->setentry(entbuffer, entrysize); // save text to module at current position
	      }
	    }
	  }

	  entbuffer[0] = 0;
	  entrysize = 0;

	  level++;
	  keysize = 0;
	  while ((c = fgetc(infile)) != EOF) {
	    if (c != '>') {
	      keybuffer[keysize] = c;
	      keysize++;
	    }
	    else {
	      break;
	    }
	  }
	  keybuffer[keysize] = 0;
	  
	  type[0] = 0;
	  strtmp = strstr(keybuffer, "type=\"");
	  if (strtmp) {
	    strtmp += 6;
	    i = 0;
	    for (;*strtmp != '\"'; strtmp++) {
	      if (*strtmp == 10) {
		type[i] = ' ';
		i++;
	      }
	      else if (*strtmp != 13) {
		type[i] = *strtmp;
		i++;
	      }
	    }
	    type[i] = 0;
	  }
	  
	  n[0] = 0;
	  strtmp = strstr(keybuffer, "n=\"");
	  if (strtmp) {
	    strtmp += 3;
	    i = 0;
	    for (;*strtmp != '\"'; strtmp++) {
	      if (*strtmp == 10) {
		n[i] = ' ';
		i++;
	      }
	      else if (*strtmp != 13) {
		n[i] = *strtmp;
		i++;
	      }
	    }
	    n[i] = 0;
	  }

	  title[0] = 0;
	  strtmp = strstr(keybuffer, "title=\"");
	  if (strtmp) {
	    strtmp += 7;
	    i = 0;
	    for (;*strtmp != '\"'; strtmp++) {
	      if (*strtmp == 10) {
		title[i] = ' ';
		i++;
	      }
	      else if (*strtmp != 13) {
		title[i] = *strtmp;
		i++;
	      }
	    }
	    title[i] = 0;
	  }
	  strcpy (keybuffer, type);
	  if (strlen(keybuffer))
	    strcat (keybuffer, " ");
	  strcat (keybuffer, n);

	  if (longnames && strlen(keybuffer))
	    strcat (keybuffer, ": ");
	  if (longnames || !strlen(keybuffer))
	    strcat (keybuffer, title);

	  divs[level-1] = keybuffer;

	  closer = false;
	}
      }
    }
    else if (c != 13) {
      entbuffer[entrysize] = c;
      entrysize++;
      entbuffer[entrysize] = 0;
    }
  }
//DEBUG  printTree(root, treeKey);

  delete treeKey;
  delete book;
  return 0;
}












