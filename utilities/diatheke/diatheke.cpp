// Diatheke 3.1 by Chris Little <chrislit@gotjesus.org>
// Copyright 1999 by Chris Little
// Licensed under GNU General Public License (GPL)
// see accompanying LICENSE file for license details

#include <stdio.h>
#include <rawtext.h>
#include <swmgr.h>
#include <iostream.h>
#include <time.h>

#include <gbfthml.h>

#include <thmlgbf.h>
#include <thmlplain.h>
#include <thmlhtml.h>
#include <thmlrtf.h>

#include <localemgr.h>

// if you want to do logging of lookups, uncomment this #define and make /var/log/diatheke.log world writeable (chmod 666 it)
//#define LOGGING

int hasalpha (char * string) {
  for (; *string != 0; string++)
    if (isalpha(*string))
      return 1;
  return 0;
}


int main(int argc, char **argv)
{
	SWMgr manager;
	SWModule *target;
	ListKey listkey;
	//VerseKey *vk;
	ModMap::iterator it;
	SectionMap::iterator sit;
	ConfigEntMap::iterator eit;
	int maxverses;
	SWFilter * filter = 0;
	int length;
	char *font = 0;
	bool GBF = 0;
	bool ThML = 0;

	if (argc > 1) {
	  char * flags = argv[1];

	  if (argc > 3) {
	    char * book = argv[2];
	    char * ref = argv[3]; // ie the lexicon key, a verse reference, or search string
  
	    it = manager.Modules.find(book);
	    target = (*it).second;	  
	    
	    if ((sit = manager.config->Sections.find((*it).second->Name())) != manager.config->Sections.end()) {
	      if ((eit = (*sit).second.find("SourceType")) != (*sit).second.end()) {
		if (!strcmp((char *)(*eit).second.c_str(), "GBF"))
		  GBF = 1;
		else if (!strcmp((char *)(*eit).second.c_str(), "ThML"))
		  ThML = 1;
	      }
	    }
	    
	    if (argc > 4) {
	      LocaleMgr::systemLocaleMgr.setDefaultLocaleName(argv[4]);
	    }
	    if (argc > 5) {
	      if (!strcmp(argv[5], "thml")) {
	      }
	      else if (!strcmp(argv[5], "html")) {
		filter = new ThMLHTML();
	      }
	      else if (!strcmp(argv[5], "gbf")) {
		filter = new ThMLGBF();
	      }
	      else if (!strcmp(argv[5], "rtf")) {
		filter = new ThMLRTF();
	      }
	    }
	    else filter = new ThMLPlain();
	    
	    if (argc > 6)
	      maxverses = atoi(argv[6]);
	    else
	      maxverses = -1;
	    
#ifdef LOGGING
	    FILE* logfile = fopen("/var/log/diatheke.log", "a");
	    int n = time(NULL);
	    char* time = ctime((const long int *)&n);
	    time[strlen(time) - 1] = 0;
	    fprintf(logfile, time);
	    fprintf(logfile, " ");
	    n = 0;
	    while (argv[n] != NULL) {
	      fprintf(logfile, argv[n]);
	      fprintf(logfile, " | ");
	      n++;
	    }
#endif
	    
	    if (strchr (flags, 'f'))
	      manager.setGlobalOption("Footnotes","On");
	    else
	      manager.setGlobalOption("Footnotes","Off");
	    if (strchr (flags, 'n'))
	      manager.setGlobalOption("Strong's Numbers","On");
	    else
	      manager.setGlobalOption("Strong's Numbers","Off");
	    
	    if (strchr (flags, 's')) {
	      //do search stuff
	      
	      int searchtype;
	      if (*ref == '@') { //regex
		searchtype = 0;
		ref++;
	      }
	      else if (*ref == '#') { //multi-word
		searchtype = -2;
		ref++;
	      }
	      else searchtype = -1; //phrase
	      
	      cout << "Verse(s) containing \"" << ref << "\": ";
	      listkey = target->Search(ref, searchtype);
	      
	      if (strlen((const char*)listkey)) {
		if (!listkey.Error())
		  cout << (const char *)listkey;
		listkey++;
		while (!listkey.Error()) {
		  cout << " ; " << (const char *)listkey;
		  listkey++;
		}
		cout << " -- " << listkey.Count() << " matches total (" << target->Name() << ")" << endl;
	      }
	      else
		cout << "none";
	      
#ifdef LOGGING	    
	      fprintf (logfile, "\n");
	      fclose(logfile);
#endif
	      return(0);
	    }
	    
	    else if (strchr(flags, 'd')) {
	      //do dictionary stuff
	      
	      if (!strcmp(target->Type(), "Lexicons / Dictionaries")) {
		
		target->SetKey(ref);
		
		char * text = (char *) *target;
		if (GBF) {
		  target->AddRenderFilter(new GBFThML());
		}
		if (filter) target->AddRenderFilter(filter);
		if (strlen(text)) {
		  cout << target->KeyText() << " : " <<  text << "\n(" << target->Name() << ")" << endl;
		}
#ifdef LOGGING
		else
		  fprintf(logfile, "  FAILED\n");
		
		fprintf (logfile, "\n");
		fclose(logfile);
#endif
		return(0);
	      }
	    }
	    
	    else if (strchr(flags, 'b') || strchr(flags, 'c')) {
	      //do commentary/verse stuff
	      
	      if ((sit = manager.config->Sections.find((*it).second->Name())) != manager.config->Sections.end()) {
		if ((eit = (*sit).second.find("Font")) != (*sit).second.end()) {
		  font = (char *)(*eit).second.c_str();
		  if (strlen(font) == 0) font = 0;
		}
	      }
	      
	      if (!strcmp(target->Type(), "Biblical Texts") || !strcmp(target->Type(), "Commentaries")) {
		if (GBF) {
		  target->AddRenderFilter(new GBFThML());
		}
		if (filter) target->AddRenderFilter(filter);
		
		char * comma = strchr(ref, ',');
		char * dash = strchr(ref, '-');
		
		if (comma) {              // if we're looking up a sequence of
		  // verses (must be in same chapter)
		  char * vers1 = strchr(ref, ':') + 1;
		  
		  char vers_array[strlen(vers1)];
		  char * vers2 = (char*)vers_array;
		  strcpy (vers2, vers1);
		  
		  char chap_array[strlen(ref) + 8];
		  char * chap = (char*)chap_array;
		  strcpy (chap, ref);
		  
		  char * vers3 = strchr(chap, ':') + 1;
		  *vers3 = 0;
		  char * vers4 = new char;
		  
		  vers4 = strtok(vers2, ",");
		  while (vers4) {
		    strcpy (vers3, vers4);
		    
		    dash = strchr(chap, '-');
		    if (dash) {               // if we're looking up a range...
		      *dash = 0;              //break string at the dash
		      dash++;
		      char temp_array[strlen(chap) + 8];
		      char * temp = (char*)temp_array;
		      
		      length = strchr(chap, ':') - chap + 1;
		      
		      strncpy (temp, chap, length);
		      *(temp + length) = 0;
		      strcat (temp, dash);
		      strcpy (dash, temp);
		    }
		    else dash = chap;
		    
		    for(target->Key(chap); target->Key()<(VerseKey)dash && --maxverses;(*target)++) {
		      
		      if (font && !filter)
			cout << (char*)target->KeyText() << " " << ": <font face=\"" << font << "\">" << (char const*)*target << "</font>" << endl;
		      else 
			cout << (char*)target->KeyText() << ": " << (char const*)*target << endl;
		      if (!ThML && !filter) cout << "<br>" << endl;
		      
		    }

		    
		    if (font && !filter)
		      cout << (char*)target->KeyText() << ": <font face=\"" << font << "\">" << (char const*)*target << "</font> (" << target->Name() << ")" << endl;
		    else 
		      cout << (char*)target->KeyText() << ": " << (char const*)*target << " (" << target->Name() << ")" << endl;
		    if (!ThML && !filter) cout << "<br>" << endl;
		    
		    vers4 = strtok(0, ",");
		  }
		}
		else {
		  if (dash) {                 // if we're looking up a range...
		    *dash = 0;                //break string at the dash
		    dash++;
		    char temp_array[strlen(ref)];
		    char * temp = (char*)temp_array;
		    
		    if (!strchr (dash, ':')) { /// if range supplies only second verse number (no book/chapter) assume same book/chapter
		      length = strchr(ref, ':') - ref + 1;
		      strncpy (temp, ref, length);
		      *(temp + length) = 0;
		      strcat (temp, dash);
		      strcpy (dash, temp);
		    }
		    else if (!hasalpha (dash)){ /// if range supplies only second chapter/verse (no book--has no letters) assume same book
		      strcpy (temp, ref);
		      length = 0;
		      while (!isalpha(*temp)) {temp++; length++;}
		      while (isalpha(*temp)) {temp++; length++;}
		      while (!isdigit(*temp)) {temp++; length++;}
		      strncpy (temp, ref, length);
		      *(temp + length) = 0;
		      strcat (temp, dash);
		      strcpy (dash, temp);
		    }
		  }
		  else dash = ref;	       
		  
		  for(target->Key(ref); target->Key()<(VerseKey)dash && --maxverses;(*target)++) {
		    if (font && !filter)
		      cout << (char*)target->KeyText() << ": <font face=\"" << font << "\">" <<  (char const*)*target << "</font>" << endl;
		    else 
		      cout << (char*)target->KeyText() << ": " << (char const*)*target << endl;
		    if (!ThML && !filter) cout << "<br>" << endl;
		  }
		  if (font && !filter)
		    cout << (char*)target->KeyText() << ": <font face=\"" << font << "\">" << (char const*)*target << "</font> (" << target->Name() << ")" << endl;
		  else 
		    cout << (char*)target->KeyText() << ": " << (char const*)*target << " (" << target->Name() << ")" << endl;
		  if (!ThML && !filter) cout << "<br>" << endl;
		}
		
	      }
#ifdef LOGGING	    
	      fprintf (logfile, "\n");
	      fclose(logfile);
#endif
	      return(0);	    
	      
	    }
	  }    
	  else {
	    if (strchr(flags, 'm')) {
	      // print a list of available modules
	      SWMgr mymgr;
	      ModMap::iterator it;
	      SWModule *module;
	      
	      cout << "Biblical Texts:\n";
	      for (it = mymgr.Modules.begin(); it != mymgr.Modules.end(); it++) {
		module = it->second;
		if (!strcmp(module->Type(), "Biblical Texts")) {
		  cout << module->Name() << " : " << module->Description() << "\n";
		}
	      }
	      cout << "Commentaries:\n";
	      for (it = mymgr.Modules.begin(); it != mymgr.Modules.end(); it++) {
		module = it->second;
		if (!strcmp(module->Type(), "Commentaries")) {
		  cout << module->Name() << " : " << module->Description() << "\n";
		} 
	      }
	      cout << "Dictionaries:\n";
	      for (it = mymgr.Modules.begin(); it != mymgr.Modules.end(); it++) {
		module = it->second;
		if (!strcmp(module->Type(), "Lexicons / Dictionaries")) {
		  cout << module->Name() << " : " << module->Description() << "\n";
	 	}
	      }
		
#ifdef LOGGING	    
	      fprintf (logfile, "\n");
	      fclose(logfile);
#endif
	      return(0);
	    }
	    else if (strchr(flags, 'l')) {

	      
	      LocaleMgr lm = LocaleMgr::systemLocaleMgr;
	      list<string> loclist =  lm.getAvailableLocales();
	      copy (loclist.begin(), loclist.end(), ostream_iterator<string> (cout, "\n"));

#ifdef LOGGING	    
	      fprintf (logfile, "\n");
	      fclose(logfile);
#endif
	      return(0);
	    }
	  }
#ifdef LOGGING
	  fprintf(logfile, "  FAILED\n");
	  fclose(logfile);
#endif
	}
	
	//if we got this far without exiting, something went wrong, so print syntax
	fprintf(stderr, "Diatheke command-line Bible utility and SWORD frontend Version 3.1 by Chris Little \nusage: \n %s -m                         (print list of available modules)\n %s -b [bible] [verse]         (verse lookup)\n %s -s [book] [word]           (word search)\n %s -d [dictionary] [word]     (dictionary lookup)\n %s -c [commentary] [verse]    (commentary lookup)\n\n To turn Strong's numbers on, add 'n' after the argument '-b'.\n Likewise, add 'f' after '-b' to turn footnotes on.\n Also supported, at the end of the command line are\n locale (en, de, etc.), format (plain(default), html, gbf, rtf, or thml),\n max-verse (a number), and locale (en(default, de, es, etc.)\n arguments\n", argv[0], argv[0], argv[0], argv[0], argv[0]);
	exit(-1);
}

	





