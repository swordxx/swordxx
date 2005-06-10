#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <fstream>
using namespace std;

#if 0
#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif
#endif

#include <iostream>
#include <string>
#include <rawtext.h>


#ifndef NO_SWORD_NAMESPACE
using sword::RawText;
using sword::VerseKey;
using sword::SWText;
using sword::ListKey;
#endif
#if 0
int readline(FILE* infile, string &linebuffer) {
  signed char c;
  int lbPtr = 0;
  linebuffer = string(1048576,' ');
  while ((c = fgetc(infile)) != EOF) {
    if(lbPtr < linebuffer.size())
      linebuffer[lbPtr++] = c;
    else {
      linebuffer.resize(linebuffer.size()+1048576);
      linebuffer[lbPtr++] = c;
    }
    //lbPtr++;
    if (c == 10) {
      linebuffer.resize(lbPtr);
      return (linebuffer.size());
    }
  }
  return 0;
}
#endif

int main(int argc, char **argv) {
  
  const string helptext ="imp2vs 1.0 Bible/Commentary module creation tool for the SWORD Project\n  usage:\n   %s <filename> [output dir] \n";
  
  signed long i = 0;
  string keybuffer;
  string entbuffer;
  string linebuffer;
  string modname;
  
  if (argc > 2) {
    modname = argv[2];
  }
  else if (argc > 1) {
    modname = "./";
  }
  else {
    fprintf(stderr, helptext.c_str(), argv[0]);
    exit(-1);
  }
  
  ifstream infile(argv[1]);
  //infile = argv[1], "r");

  SWText* mod;
  RawText::createModule(modname.c_str());
  RawText modRaw(modname.c_str());
  mod = &modRaw;

  VerseKey* vkey = new VerseKey;
  vkey->Headings(1);
  vkey->AutoNormalize(0);
  vkey->Persist(1);
  mod->setKey(*vkey);
  char final; // 2 == pre-final line; 1 == final line; 0 == EOF

  getline(infile,linebuffer);
  final = (!infile.eof()) + 1;
  //final = ((bool)(readline(infile, linebuffer)) + 1);

  while (final) {
    if (linebuffer.substr(0,3) == "$$$" || final == 1) {
      if (keybuffer.size() && entbuffer.size()) {
	std::cout << "from file: " << keybuffer << std::endl;
	*vkey = keybuffer.c_str();
	if (!vkey->Chapter()) {
	  // bad hack:  0:0 is Book intro; (chapter):0 is Chapter intro; 0:2 is Module intro; 0:1 is Testament intro
	  int backstep = vkey->Verse();
	  if (backstep) {
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
       }

	  std::cout << "adding entry: " << *vkey << std::endl;
	  mod->setEntry(entbuffer.c_str(), entbuffer.size());
	}
	else {
	  ListKey listkey = vkey->ParseVerseList(keybuffer.c_str(), "Gen1:1", true);
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
	      if (!havefirst) {
		havefirst = true;
		firstverse = *vkey;

	     std::cout << "adding entry: " << *vkey << std::endl;
		mod->setEntry(entbuffer.c_str(), entbuffer.size());
		(*vkey)++;
	      }
	      while (! (finalkey < (*vkey))) {
		std::cout << "linking entry: " << *vkey << " to " << firstverse << std::endl;
		*mod << &firstverse;
		(*vkey)++;
	      }
	    }
	    else {
	      if (havefirst) {
		*vkey = (*listkey.GetElement(i));
		std::cout << "linking entry: " << *vkey << " to " << firstverse << std::endl;
		*mod << &firstverse;
	      }
	      else {
		*vkey = (*listkey.GetElement(i));
		havefirst = true;
		firstverse = *vkey;

	     std::cout << "adding entry: " << *vkey << std::endl;
		mod->setEntry(entbuffer.c_str(), entbuffer.size());
	      }
	    }
	  }
	}
      }
      //linebuffer.resize(linebuffer.size()-1);
      keybuffer = linebuffer.substr(3,linebuffer.size()) ;
      entbuffer.resize(0);
    }
    else {
      entbuffer.append(linebuffer);
    }
    final--;
    if (final) {
      getline(infile,linebuffer);
      final = (!infile.eof()) + 1;
      //final = ((bool)(readline(infile, linebuffer)) + 1);
    }
  }

  //delete entbuffer;
  //delete linebuffer;
  //delete keybuffer;

  return 0;
}
