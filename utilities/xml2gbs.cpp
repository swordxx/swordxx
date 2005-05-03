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
//#include <iostream>
#include <string>
#include <stdio.h>
#include <treekeyidx.h>
#include <rawgenbook.h>


#ifndef NO_SWORD_NAMESPACE
using sword::TreeKeyIdx;
using sword::RawGenBook;
using sword::SWKey;
#endif

#define DEBUG

/*
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
*/


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

#ifdef DEBUG
//      std::cout << treeKey->getLocalName() << " : " << tok << std::endl;
#endif
      
      tok = strtok(NULL, "/");
      
    }
}

int readline(FILE* infile, char* linebuffer) {
  signed char c;
  char* lbPtr = linebuffer;
  while ((c = fgetc(infile)) != EOF) {
    *lbPtr++ = c;
    if (c == 10) {
      *lbPtr = 0;
      return (lbPtr-linebuffer);
    }
  }
  return 0;
}

enum XML_FORMATS { F_AUTODETECT, F_OSIS, F_THML };

#define HELPTEXT "xml2gbs 1.0 OSIS/ThML General Book module creation tool for the SWORD Project\n  usage:\n   xml2gbs [-l] [-i] [-fT|-fO] <filename> [modname]\n  -l uses long div names in ThML files\n  -i exports to IMP format instead of creating a module\n  -fO and -fT will set the importer to expect OSIS or ThML format respectively\n    (otherwise it attempts to autodetect)\n"

unsigned char detectFormat(char* filename, char* entbuffer) {

  unsigned char format = F_AUTODETECT;

  FILE *infile;
  infile = fopen(filename, "r");
  if (!infile) {
        fprintf(stderr, HELPTEXT);
        fprintf(stderr, "\n\nCould not open file \"%s\"\n", filename);
  }
  else {
        while (readline(infile, entbuffer) && format == F_AUTODETECT) {
                if (strstr(entbuffer, "<osis")) {
                        format = F_OSIS;
                }
                else if (strstr(entbuffer, "<ThML")) {
                        format = F_THML;
                }
        }
        fclose(infile);
  }

  return format;
}

int getTag(FILE* file, char* keybuffer) {
        char c;
        char* kbPtr = keybuffer;
        while ((c = fgetc(file)) != '>')
                *kbPtr++ = c;
        *kbPtr++ = c;
        *kbPtr = 0;
        return (kbPtr-keybuffer);
}

int processXML(char* filename, char* modname, bool longnames, bool exportfile, unsigned char format, char* entbuffer) {
  signed long i = 0;
  char* strtmp;

#ifdef DEBUG
  printf ("%s :%s :%d :%d :%d\n\n", filename, modname, longnames, exportfile, format);
#endif

  FILE *infile;
  infile = fopen(filename, "r");
  if (!infile) {
        fprintf(stderr, HELPTEXT);
        fprintf(stderr, "\n\nCould not open file \"%s\"\n", filename);
        return -1;
  }
  FILE *outfile;
  if (exportfile) {
    strcat (modname, ".imp");
    outfile = fopen(modname, "w");
  }

  TreeKeyIdx * treeKey;
  RawGenBook * book;

  std::string divs[32];

  int level = 0;
  char* keybuffer = new char[2048];
  char* keybuffer2 = new char[2048];
  char* n = new char[256];
  char* type = new char[256];
  char* title= new char[512];
  unsigned long entrysize = 0;
  unsigned long keysize = 0;
  bool closer = false;

  if (!exportfile) {
    // Do some initialization stuff
    TreeKeyIdx::create(modname);
    treeKey = new TreeKeyIdx(modname);
    RawGenBook::createModule(modname);
    delete treeKey;
    book = new RawGenBook(modname);
    treeKey = ((TreeKeyIdx *)((SWKey *)(*book)));
  }

#ifdef DEBUG
//  TreeKeyIdx root = *((TreeKeyIdx *)((SWKey *)(*book)));
#endif

  int c;
  while ((c = fgetc(infile)) != EOF) {
    if (c == '<') {
      if (getTag(infile, keybuffer)) {
	if ((format == F_OSIS) && ((!strcmp(keybuffer, "/div>")) || (!strcmp(keybuffer, "/verse>"))) ||
           ((format == F_THML) && ((!strncmp(keybuffer, "/div", 4)) && (keybuffer[4] > '0' && keybuffer[4] < '7')))) {
	  if (!closer) {
       	    keysize = 0;
            keybuffer2[0] = 0;
       	    for (i = 0; i < level; i++) {
              keybuffer2[keysize] = '/';
       	      keysize++;
              keybuffer2[keysize] = 0;
       	      strcat (keybuffer2, divs[i].c_str());
              keysize += divs[i].length();
       	    }

	    if (level) {
	      printf ("%s\n", keybuffer2);
	      if (exportfile) {
		fprintf (outfile, "$$$%s\n%s\n", keybuffer2, entbuffer);
	      }
	      else {
		treeKey->root();
		setkey(treeKey, keybuffer2);
		book->setEntry(entbuffer, entrysize); // save text to module at current position
	      }
	    }
	  }
	  level--;
	  entbuffer[0] = 0;
	  entrysize = 0;

	  closer = true;
	}
	else if ((format == F_OSIS) && !((!strcmp(keybuffer, "div>") || !strncmp(keybuffer, "div ", 4)) || (!strcmp(keybuffer, "verse>") || !strncmp(keybuffer, "verse ", 6))) ||
                ((format == F_THML) && !((!strncmp(keybuffer, "div", 3)) && (keybuffer[3] > '0' && keybuffer[3] < '7')))) {
	  entbuffer[entrysize++] = '<';
	  for (i = 0; i <= strlen(keybuffer); i++) {
              entbuffer[entrysize++] = keybuffer[i];
	  }
          entrysize--;
	}
	else {
	  //we have a divN...
       	  if (!closer) {
            keysize = 0;
       	    keybuffer2[0] = 0;
       	    for (i = 0; i < level; i++) {
              keybuffer2[keysize] = '/';
       	      keysize++;
              keybuffer2[keysize] = 0;
       	      strcat (keybuffer2, divs[i].c_str());
              keysize += divs[i].length();
       	    }

	    if (level) {
	      printf ("%s\n", keybuffer2);
	      if (exportfile) {
		fprintf (outfile, "$$$%s\n%s\n", keybuffer2, entbuffer);
	      }
	      else {
		treeKey->root();
		setkey(treeKey, keybuffer2);
		book->setEntry(entbuffer, entrysize); // save text to module at current position
	      }
	    }
	  }

	  entbuffer[0] = 0;
	  entrysize = 0;

	  level++;
          keysize = strlen(keybuffer)-1;
/*	  keysize = 0;
	  while ((c = fgetc(infile)) != EOF) {
	    if (c != '>') {
	      keybuffer[keysize] = c;
	      keysize++;
	    }
	    else {
	      break;
	    }
	  }
	  keybuffer[keysize] = 0;*/

          type[0] = 0;
      	  n[0] = 0;
       	  title[0] = 0;

          if (format == F_OSIS && longnames == false) {
               	  strtmp = strstr(keybuffer, "osisID=\"");
               	  if (strtmp) {
               	    strtmp += 8;
               	    i = 0;
               	    for (;*strtmp != '\"'; strtmp++) {
               	      if (*strtmp == 10) {
               		title[i] = ' ';
               		i++;
               	      }
               	      else if (*strtmp == '.') {
                        i = 0;
               	      }
               	      else if (*strtmp != 13) {
               		title[i] = *strtmp;
               		i++;
               	      }
               	    }
               	    title[i] = 0;
               	  }
                  strcpy (keybuffer, title);
          }
          else {
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

                  if (format == F_OSIS) {
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
                  }
                  else if (format == F_THML) {
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
                  }

        	  strcpy (keybuffer, type);
        	  if (strlen(keybuffer) && strlen(n))
        	    strcat (keybuffer, " ");
        	  strcat (keybuffer, n);

        	  if (longnames && strlen(keybuffer))
        	    strcat (keybuffer, ": ");
        	  if (longnames || !strlen(keybuffer))
        	    strcat (keybuffer, title);
          }
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

#ifdef DEBUG
//  printTree(root, treeKey);
#endif

//  delete book;  //causes nasty-bad errors upon execution
  delete n;
  delete type;
  delete title;
  delete keybuffer;
}

int main(int argc, char **argv) {
  unsigned long i = 0;

  char modname[256];
  *modname = 0;
  char filename[256];
  *filename = 0;

  bool longnames = false;
  bool exportfile = false;
  unsigned char format = F_AUTODETECT;

  if (argc > 2) {
        for (i = 1; i < argc; i++) {
                if (argv[i][0] == '-') {
                        switch (argv[i][1]) {
                                case 'l':
                                        longnames = true;
                                        continue;
                                case 'i':
                                        exportfile = true;
                                        continue;
                                case 'f':
                                        if (argv[i][2] == 'O') {
                                                format = F_OSIS;
                                        }
                                        else if (argv[i][2] == 'T') {
                                                format = F_OSIS;
                                        }
                                        else {
                                                format = F_AUTODETECT;
                                        }
                                        continue;
                        }
                }
                else if (*filename == 0) {
                        strcpy (filename, argv[i]);
                }
                else if (*modname == 0) {
                        strcpy (modname, argv[i]);
                }
        }
  }
  else if (argc > 1) {
    strcpy (filename, argv[1]);
  }

  if (!*filename) {
    fprintf(stderr, HELPTEXT);
    return -1;
  }
  else {
        if (!*modname) {
                for (i = 0; (i < 256) && (filename[i]) && (filename[i] != '.'); i++) {
                        modname[i] = filename[i];
                }
                modname[i] = 0;
        }

        char* entbuffer = new char[1048576];
        format = (format == F_AUTODETECT) ? detectFormat(filename, entbuffer) : format;
        if (format == F_AUTODETECT) {
                fprintf(stderr, HELPTEXT);
                fprintf(stderr, "\n\nCould not detect file format for file \"%s\", please specify.\n", filename);
                return -1;
        }

        int retCode =  processXML (filename, modname, longnames, exportfile, format, entbuffer);
        delete entbuffer;

        return retCode;
  }
}



