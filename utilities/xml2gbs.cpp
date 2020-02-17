/******************************************************************************
 *
 *  xml2gbs.cpp -    Importer for GenBooks formatted as OSIS, ThML, or TEI
 *
 * Copyright 2003-2012 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <swordxx/keys/treekeyidx.h>
#include <swordxx/modules/genbook/rawgenbook.h>
#include <swordxx/utilstr.h>


using swordxx::TreeKeyIdx;
using swordxx::RawGenBook;
using swordxx::SWKey;
using swordxx::startsWith;

enum XML_FORMATS { F_AUTODETECT, F_OSIS, F_THML, F_TEI };

#define HELPTEXT "xml2gbs 1.0 OSIS/ThML/TEI General Book module creation tool for the Sword++ Project\n  usage:\n   xml2gbs [-l] [-i] [-fT|-fO|-fE] <filename> [modname]\n  -l uses long div names in ThML files\n  -i exports to IMP format instead of creating a module\n  -fO, -fT, and -fE will set the importer to expect OSIS, ThML, or TEI format respectively\n    (otherwise it attempts to autodetect)\n"

unsigned char detectFormat(char* filename) {

  unsigned char format = F_AUTODETECT;

  std::ifstream infile(filename);
  std::string entbuffer;

  if (!infile.is_open()) {
        std::cerr << HELPTEXT;
        std::cerr << std::endl << std::endl << "Could not open file \"" << filename << "\"" << std::endl;
  }
  else {
    while (std::getline(infile, entbuffer) && format == F_AUTODETECT) {
                if (std::strstr(entbuffer.c_str(), "<osis")) {
                        format = F_OSIS;
                }
                else if (std::strstr(entbuffer.c_str(), "<ThML")) {
                        format = F_THML;
                }
        else if (std::strstr(entbuffer.c_str(), "<TEI")) {
                        format = F_TEI;
        }
        }
        infile.close();
  }

  return format;
}

int processXML(const char* filename, char* modname, bool longnames, bool exportfile, unsigned char format) {
  using namespace std::literals::string_view_literals;

  signed long i = 0;
  char* strtmp;
  std::string entbuffer;

  std::ifstream infile(filename);
  if (!infile.is_open()) {
        std::cerr << HELPTEXT;
        std::cerr << std::endl << std::endl << "Could not open file \"" << filename << "\"" << std::endl;
        return -1;
  }
  std::ofstream outfile;
  if (exportfile) {
    std::strcat(modname, ".imp");
    outfile.open(modname);
  }

  std::unique_ptr<RawGenBook> book;

  std::string divs[32];

  int level = 0;
  std::string keybuffer = "";
  std::string keybuffer2;
  std::string n;
  std::string type;
  std::string title;
  unsigned long entrysize = 0;
  unsigned long keysize = 0;
  bool closer = false;

  if (!exportfile) {
    // Do some initialization stuff
    TreeKeyIdx::create(modname);
    {
        auto const treeKey(std::make_unique<TreeKeyIdx>(modname));
        RawGenBook::createModule(modname);
    }
    book = std::make_unique<RawGenBook>(modname);
  }

  int c;
  while ((c = infile.get()) != EOF) {
    if (c == '<') {
        {
            keybuffer = "";
            while ((c = infile.get()) != '>')
                keybuffer += c;
            keybuffer += c;
        }

      if (keybuffer.length()) {
    if (((format == F_OSIS) && ((startsWith(keybuffer, "/div>"sv)) || (startsWith(keybuffer, "/verse>"sv)) || (startsWith(keybuffer, "/chapter>"sv)))) ||
           ((format == F_THML) && ((startsWith(keybuffer, "/div"sv)) && (keybuffer[4] > '0' && keybuffer[4] < '7')))) {
      if (!closer) {
               keysize = 0;
            keybuffer2 = "";
               for (i = 0; i < level; i++) {
              keybuffer2 += '/';
                 keysize++;
                 keybuffer2 += divs[i];
              keysize += divs[i].length();
          std::cout << keybuffer2 << std::endl;
               }

        if (level) {
          std::cout << keybuffer2 << std::endl;
          if (exportfile) {
        outfile << "$$$" << keybuffer2 << std::endl << entbuffer << std::endl;
          }
          else {
        book->setKey(keybuffer2.c_str());
        book->setEntry(std::string_view(entbuffer.c_str(), entrysize)); // save text to module at current position
          }
        }
      }
      level--;
      entbuffer = "";
      entrysize = 0;

      closer = true;
    }
    else if (((format == F_OSIS) && !((startsWith(keybuffer, "div>"sv) || startsWith(keybuffer, "div "sv)) || (startsWith(keybuffer, "verse>"sv) || startsWith(keybuffer, "verse "sv)) || (startsWith(keybuffer, "chapter>"sv) || startsWith(keybuffer, "chapter "sv)))) ||
                ((format == F_THML) && !((startsWith(keybuffer, "div"sv)) && (keybuffer[3] > '0' && keybuffer[3] < '7')))) {
      entbuffer += '<';
      entrysize++;
      entrysize += keybuffer.length();
      entbuffer += keybuffer;
    }
    else {
      //we have a divN...
             if (!closer) {
            keysize = 0;
               keybuffer2= "";
               for (i = 0; i < level; i++) {
              keybuffer2 += '/';
                 keysize++;
                 keybuffer2 += divs[i];
              keysize += divs[i].length();
          std::cout << keybuffer2 << std::endl;
               }

        if (level) {
          std::cout << keybuffer2 << std::endl;
          if (exportfile) {
        outfile << "$$$" << keybuffer2 << std::endl << entbuffer << std::endl;
          }
          else {
        book->setKey(keybuffer2.c_str());
        book->setEntry(std::string_view(entbuffer.c_str(), entrysize)); // save text to module at current position
          }
        }
      }

      entbuffer= "";
      entrysize = 0;

      level++;
          keysize = keybuffer.length()-1;

          type = "";
            n = "";
             title = "";

          if (format == F_OSIS && longnames == false) {
                     strtmp = (char*)std::strstr(keybuffer.c_str(), "osisID=\"");
                     if (strtmp) {
                       strtmp += 8;
                       for (;*strtmp != '\"'; strtmp++) {
                         if (*strtmp == 10) {
                       title += ' ';
                         }
                         else if (*strtmp == '.') {
                        title = "";
                         }
                         else if (*strtmp != 13) {
                       title += *strtmp;
                         }
                       }
                     }
                  keybuffer = title;
          }
          else {
                     strtmp = (char*)std::strstr(keybuffer.c_str(), "type=\"");
                     if (strtmp) {
                       strtmp += 6;
                       for (;*strtmp != '\"'; strtmp++) {
                         if (*strtmp == 10) {
                       type+= ' ';
                         }
                         else if (*strtmp != 13) {
                       type+= *strtmp;
                         }
                       }
                     }

                     strtmp = (char*)std::strstr(keybuffer.c_str(), "n=\"");
                     if (strtmp) {
                       strtmp += 3;
                       for (;*strtmp != '\"'; strtmp++) {
                         if (*strtmp == 10) {
                       n += ' ';
                         }
                         else if (*strtmp != 13) {
                       n += *strtmp;
                         }
                       }
                     }

                  if (format == F_OSIS) {
                             strtmp = (char*)std::strstr(keybuffer.c_str(), "title=\"");
                      if (strtmp) {
                        strtmp += 7;
                        for (;*strtmp != '\"'; strtmp++) {
                          if (*strtmp == 10) {
                        title += ' ';
                          }
                          else if (*strtmp != 13) {
                        title += *strtmp;
                          }
                        }
                      }
                  }
                  else if (format == F_THML) {
                      strtmp = (char*)std::strstr(keybuffer.c_str(), "title=\"");
                      if (strtmp) {
                        strtmp += 7;
                        for (;*strtmp != '\"'; strtmp++) {
                          if (*strtmp == 10) {
                        title += ' ';
                          }
                          else if (*strtmp != 13) {
                        title += *strtmp;
                          }
                        }
                      }
                  }

              keybuffer = type;
              if (keybuffer.length() && n.length())
                keybuffer += " ";
              keybuffer += n;

              if (longnames && keybuffer.length())
                keybuffer += ": ";
              if (longnames || !keybuffer.length())
                keybuffer += title;
          }
          divs[level-1] = keybuffer;

      closer = false;
    }
      }
    }
    else if (c != 13) {
      entbuffer += c;
      entrysize++;
    }
  }

    return 0;
}

int main(int argc, char **argv) {
  int i = 0;

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
                        std::strncpy (filename, argv[i], 200);
                }
                else if (*modname == 0) {
                        std::strncpy (modname, argv[i], 200);
                }
        }
  }
  else if (argc > 1) {
    std::strncpy (filename, argv[1], 200);
  }

  if (!*filename) {
    std::cerr << HELPTEXT << std::endl;
    return -1;
  }
  else {
        if (!*modname) {
                for (i = 0; (i < 256) && (filename[i]) && (filename[i] != '.'); i++) {
                        modname[i] = filename[i];
                }
                modname[i] = 0;
        }

        format = (format == F_AUTODETECT) ? detectFormat(filename) : format;
        if (format == F_AUTODETECT) {
                fprintf(stderr, HELPTEXT);
                fprintf(stderr, "\n\nCould not detect file format for file \"%s\", please specify.\n", filename);
                return -1;
        }

        int retCode =  processXML (filename, modname, longnames, exportfile, format);

        return retCode;
  }
}



