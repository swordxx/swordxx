// Diatheke 4.0 by Chris Little <chrislit@crosswire.org>
// Copyright 1999, 2000, 2001 by CrossWire Bible Society http://www.crosswire.org
// Licensed under GNU General Public License (GPL)
// see accompanying LICENSE file for license details

#include "corediatheke.h"

#define RQ_REF 1
#define RQ_BOOK 2

void printsyntax() { 
	//if we got this far without exiting, something went wrong, so print syntax
	fprintf (stderr, "Diatheke command-line SWORD frontend Version 4.0 by Chris Little \n");
	fprintf (stderr, "usage: \n  ");
	fprintf (stderr, "diatheke <-b book> [-s search_type] [-o option_filters]\n");
	fprintf (stderr, "[-m maximum_verses] [-f output_format] [-l locale] <-k query_key>\n");
	fprintf (stderr, "\n");
	fprintf (stderr, "If <book> is \"system\" you may use these system keys: \"modulelist\",\n");
	fprintf (stderr, "\"modulelistnames\", and \"localelist\".");
	fprintf (stderr, "\n");
	fprintf (stderr, "Valid search_type values are: regex, multiword, and phrase(def).\n");
	fprintf (stderr, "Valid option_filters values are: n (Strong's numbers),\n");
	fprintf (stderr, "  f (Footnotes), and h (Section Headings)\n");
	fprintf (stderr, "Maximum verses may be any integer value\n");
	fprintf (stderr, "Valid output_format values are: GBF, ThML, RTF, HTML, and plain(def)\n");
	fprintf (stderr, "Valid locale values depend on installed locales. en is default.\n");
	fprintf (stderr, "The query_key must be the last argument because all following\n");
	fprintf (stderr, "  arguments are added to the key.\n");
}

int main(int argc, char **argv)
{
	
	int maxverses = -1;
	char outputformat = FMT_PLAIN, optionfilters = OP_NONE, searchtype = ST_NONE;
	char *text = 0, *locale = 0, *ref = 0;
	
	char runquery = 0; // used to check that we have enough arguments to perform a legal query
	// (a querytype & text = 1 and a ref = 2)
	
	for (int i = 1; i < argc; i++) {
		if (!stricmp("-b", argv[i])) {
			if (i+1 <= argc) {
				text = argv[i+1];
				i++;
				runquery |= RQ_BOOK;
			}
		}
		else if (!stricmp("-s", argv[i])) {
			if (i+1 <= argc) {
				if (!stricmp("regex", argv[i+1])) {
					searchtype = ST_REGEX;
					i++;
				}
				else if (!stricmp("multiword", argv[i+1])) {
					searchtype = ST_MULTIWORD;
					i++;
				}
				else i++;
			}
		}
		else if (!stricmp("-l", argv[i])) {
			if (i+1 <= argc) {
				locale = argv[i+1];
				i++;
			}
		}
		else if (!stricmp("-m", argv[i])) {
			if (i+1 <= argc) {
				maxverses = atoi(argv[i+1]);
				i++;
			}
		}
		else if (!stricmp("-o", argv[i])) {
			if (i+1 <= argc) {
				if (strchr(argv[i+1], 'f'))
					optionfilters |= OP_FOOTNOTES;
				if (strchr(argv[i+1], 'n'))
					optionfilters |= OP_STRONGS;
				if (strchr(argv[i+1], 'h'))
					optionfilters |= OP_HEADINGS;
				if (strchr(argv[i+1], 'm'))
					optionfilters |= OP_MORPH;
				i++;
			}
		}
		else if (!stricmp("-f", argv[i])) {
			if (i+1 <= argc) {
				if (!stricmp("thml", argv[i+1])) {
					outputformat = FMT_THML;
					i++;
				}
				else if (!stricmp("gbf", argv[i+1])) {
					outputformat = FMT_GBF;
					i++;
				}
				else if (!stricmp("html", argv[i+1])) {
					outputformat = FMT_HTML;
					i++;
				}
				else if (!stricmp("rtf", argv[i+1])) {
					outputformat = FMT_RTF;
					i++;
				}
				else i++;
			}
		}
		else if (!stricmp("-k", argv[i])) {
			i++;	
			if (i < argc) {
				string key = argv[i];
				i++;
				for (; i < argc; i++)
					key = key + " " + argv[i];
				ref = new char[key.length() + 1];
				strcpy (ref, key.c_str());
				if (strlen(ref))
					runquery |= RQ_REF;
			}
		}
	}
	
	
	if (runquery == (RQ_BOOK | RQ_REF))
	{
		char * returnstring  = doquery(maxverses, outputformat, optionfilters, searchtype, text, locale, ref);
		printf ("%s", returnstring);
		delete [] returnstring;
	}
	else
		printsyntax();
}
