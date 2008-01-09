#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <fstream>
using namespace std;

#include <iostream>
#include <rawtext.h>
 #include <rawtext4.h>
#include <versekey.h>


#ifndef NO_SWORD_NAMESPACE
using sword::RawText;
using sword::RawText4;
using sword::VerseKey;
using sword::SWText;
using sword::ListKey;
#endif

int main(int argc, char **argv) {
  
  const string helptext ="imp2vs 1.0 Bible/Commentary module creation tool for the SWORD Project\n  usage:\n   %s <filename> [output dir] \n";
  
  string keybuffer = "";
  string entbuffer = "";
  string linebuffer = "";
  string modname = "";
  
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
  int mode = 1;
  if (argc > 3) {
    switch (*argv[3]) {
    case '4':
      mode = 2;
      break;
    default:
      mode = 1;
    }
  }
  


  try {
  
  ifstream infile(argv[1]);

  SWText* mod;
  RawText * modRaw;
  RawText4 * modRaw4;
  
  if (mode == 1) {
          RawText::createModule(modname.c_str());
          modRaw= new RawText(modname.c_str());
          mod = modRaw;
  } else {
          RawText4::createModule(modname.c_str());
          modRaw4= new RawText4(modname.c_str());
          mod = modRaw4;
  }

  VerseKey* vkey = new VerseKey;
  vkey->Headings(1);
  vkey->AutoNormalize(0);
  vkey->Persist(1);
  mod->setKey(*vkey);
  char final; // 2 == pre-final line; 1 == final line; 0 == EOF

  getline(infile,linebuffer);
  final = (!infile.eof()) + 1;

  while (final) {
    if (final == 1 || (linebuffer.size() > 3 && linebuffer.substr(0,3) == "$$$")) {
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

	  std::cout << "adding entry: " << *vkey << " length " << entbuffer.size() << "/" << (unsigned short)entbuffer.size() << std::endl;
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

		std::cout << "adding entry: " << *vkey << " length " << entbuffer.size() << "/" << (unsigned short)entbuffer.size() << std::endl;
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

		std::cout << "adding entry: " << *vkey << " length " << entbuffer.size() << "/" << (unsigned short)entbuffer.size() << std::endl;
		mod->setEntry(entbuffer.c_str(), entbuffer.size());
	      }
	    }
	  }
	}
      }
      if (linebuffer.size() > 3)
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
    }
  }
  }
  catch (const std::exception& e) {
         std::cerr << "Exception: imp2vs failed: " << e.what() << std::endl;
	 std::cerr << "Line: " << linebuffer.size() << " " << linebuffer << std::endl;
	 std::cerr << "Key: "  << keybuffer.size() << " " << keybuffer << std::endl;
	 std::cerr << "Ent: "  << entbuffer.size() << " " << entbuffer << std::endl;
         return -2;
  }
  catch (...) {
         std::cerr << "Exception: imp2vs failed" << std::endl;
         return -3;
  }

  return 0;
}
