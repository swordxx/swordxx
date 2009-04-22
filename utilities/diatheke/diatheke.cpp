// Diatheke 4.2.1 by Chris Little <chrislit@crosswire.org>
// Copyright 1999-2005 by CrossWire Bible Society
// http://www.crosswire.org/sword/diatheke

/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
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

#include "corediatheke.h"
#include "diathekemgr.h"
#include "diafiltmgr.h"
#include <utilstr.h>

using std::cout;

#define RQ_REF 1
#define RQ_BOOK 2

void printsyntax() { 
	//if we got this far without exiting, something went wrong, so print syntax
	fprintf (stderr, "Diatheke command-line SWORD frontend Version 4.2.1\n");
	fprintf (stderr, "Copyright 1999-2005 by the CrossWire Bible Society\n");
	fprintf (stderr, "http://www.crosswire.org/sword/diatheke/\n");
	fprintf (stderr, "usage: \n  ");
 	fprintf (stderr, "diatheke <-b module_name> [-s search_type] [-r search_range]\n");
	fprintf (stderr, "[-o option_filters] [-m maximum_verses] [-f output_format]\n");
	fprintf (stderr, "[-e output_encoding] [-t script] [-v variant#(-1=all|0|1)]\n");
	fprintf (stderr, "[-l locale] <-k query_key>\n");
	fprintf (stderr, "\n");
	fprintf (stderr, "If <book> is \"system\" you may use these system keys: \"modulelist\",\n");
	fprintf (stderr, "\"modulelistnames\", and \"localelist\".");
	fprintf (stderr, "\n");
	fprintf (stderr, "Valid search_type values are: regex, multiword, and phrase(def).\n");
	fprintf (stderr, "Valid option_filters values are: n (Strong's numbers),\n");
	fprintf (stderr, "  f (Footnotes), m (Morphology), h (Section Headings),\n");
	fprintf (stderr, "  c (Cantillation), v (Hebrew Vowels), a (Greek Accents), p (Arabic Vowels)\n");
	fprintf (stderr, "  l (Lemmas), s (Scripture Crossrefs), r (Arabic Shaping,\n");
	fprintf (stderr, "  b (Bi-Directional Reordering), x (Red Words of Christ)\n");

	fprintf (stderr, "Maximum verses may be any integer value\n");
	fprintf (stderr, "Valid output_format values are: GBF, ThML, RTF, HTML, OSIS, CGI, and plain (def)\n");
	fprintf (stderr, "Valid output_encoding values are: Latin1, UTF8 (def), UTF16, HTML, and RTF\n");
	fprintf (stderr, "Valid locale values depend on installed locales. en is default.\n");
	fprintf (stderr, "The query_key must be the last argument because all following\n");
	fprintf (stderr, "  arguments are added to the key.\n\n");

	fprintf (stderr, "Example usage:\n");
	fprintf (stderr, "  diatheke -b KJV -o fmnx -k Jn 3:16\n");
	fprintf (stderr, "  diatheke -b WHNU -t Latin -o mn -k Mt 24\n");
	fprintf (stderr, "  diatheke -b KJV -s phrase -r Mt -k love\n");
}

int main(int argc, char **argv)
{
	int maxverses = -1;
	unsigned char outputformat = FMT_PLAIN, searchtype = ST_NONE, outputencoding = ENC_UTF8;
	unsigned long optionfilters = OP_NONE;
 	char *text = 0, *locale = 0, *ref = 0, *script = 0, *range = 0;
	signed short variants = 0;
	
	char runquery = 0; // used to check that we have enough arguments to perform a legal query
	// (a querytype & text = 1 and a ref = 2)
	
	for (int i = 1; i < argc; i++) {
		if (!::stricmp("-b", argv[i])) {
			if (i+1 <= argc) {
				text = argv[i+1];
				i++;
				runquery |= RQ_BOOK;
			}
		}
		else if (!::stricmp("-s", argv[i])) {
			if (i+1 <= argc) {
				if (!::stricmp("phrase", argv[i+1])) {
					searchtype = ST_PHRASE;
					i++;
				}
				else if (!::stricmp("regex", argv[i+1])) {
					searchtype = ST_REGEX;
					i++;
				}
				else if (!::stricmp("multiword", argv[i+1])) {
					searchtype = ST_MULTIWORD;
					i++;
				}
				else i++;
			}
		}
 		else if (!::stricmp("-r", argv[i])) {
 			if (i+1 <= argc) {
 				range = argv[i+1];
 				i++;
 			}	
 		}
		else if (!::stricmp("-l", argv[i])) {
			if (i+1 <= argc) {
				locale = argv[i+1];
				i++;
			}
		}
		else if (!::stricmp("-m", argv[i])) {
			if (i+1 <= argc) {
				maxverses = atoi(argv[i+1]);
				i++;
			}
		}
		else if (!::stricmp("-o", argv[i])) {
			if (i+1 <= argc) {
				if (strchr(argv[i+1], 'f'))
					optionfilters |= OP_FOOTNOTES;
				if (strchr(argv[i+1], 'n'))
					optionfilters |= OP_STRONGS;
				if (strchr(argv[i+1], 'h'))
					optionfilters |= OP_HEADINGS;
				if (strchr(argv[i+1], 'm'))
					optionfilters |= OP_MORPH;
				if (strchr(argv[i+1], 'c'))
					optionfilters |= OP_CANTILLATION;
				if (strchr(argv[i+1], 'v'))
					optionfilters |= OP_HEBREWPOINTS;
				if (strchr(argv[i+1], 'a'))
					optionfilters |= OP_GREEKACCENTS;
				if (strchr(argv[i+1], 'l'))
					optionfilters |= OP_LEMMAS;
				if (strchr(argv[i+1], 's'))
					optionfilters |= OP_SCRIPREF;
				if (strchr(argv[i+1], 'r'))
					optionfilters |= OP_ARSHAPE;
				if (strchr(argv[i+1], 'b'))
					optionfilters |= OP_BIDI;
				if (strchr(argv[i+1], 'x'))
					optionfilters |= OP_RED;
				if (strchr(argv[i+1], 'p'))
					optionfilters |= OP_ARABICPOINTS;
				i++;
			}
		}
		else if (!::stricmp("-f", argv[i])) {
			if (i+1 <= argc) {
				if (!::stricmp("thml", argv[i+1])) {
					outputformat = FMT_THML;
					i++;
				}
				else if (!::stricmp("cgi", argv[i+1])) {
					outputformat = FMT_CGI;
					i++;
				}
				else if (!::stricmp("gbf", argv[i+1])) {
					outputformat = FMT_GBF;
					i++;
				}
				else if (!::stricmp("html", argv[i+1])) {
					outputformat = FMT_HTML;
					i++;
				}
				else if (!::stricmp("rtf", argv[i+1])) {
					outputformat = FMT_RTF;
					i++;
				}
				else if (!::stricmp("osis", argv[i+1])) {
					outputformat = FMT_OSIS;
					i++;
				}
				else i++;
			}
		}
		else if (!::stricmp("-e", argv[i])) {
			if (i+1 <= argc) {
				if (!::stricmp("utf8", argv[i+1])) {
					outputencoding = ENC_UTF8;
					i++;
				}
				else if (!::stricmp("rtf", argv[i+1])) {
					outputencoding = ENC_RTF;
					i++;
				}
				else if (!::stricmp("html", argv[i+1])) {
					outputencoding = ENC_HTML;
					i++;
				}
				else if (!::stricmp("latin1", argv[i+1])) {
					outputencoding = ENC_LATIN1;
					i++;
				}
				else if (!::stricmp("utf16", argv[i+1])) {
					outputencoding = ENC_UTF16;
					i++;
				}
				else i++;
			}
		}
		else if (!::stricmp("-k", argv[i])) {
			i++;	
			if (i < argc) {
				SWBuf key = argv[i];
				i++;
				for (; i < argc; i++)
					key = key + " " + argv[i];
				ref = new char[key.length() + 1];
				strcpy (ref, key.c_str());
				if (strlen(ref))
					runquery |= RQ_REF;
			}
		}
		else if (!::stricmp("-v", argv[i])) {
			if (i+1 <= argc) {
				variants = atoi(argv[i+1]);
				optionfilters |= OP_VARIANTS;
				i++;
			}
		}
		else if (!::stricmp("-t", argv[i])) {
			if (i+1 <= argc) {
				script = argv[i+1];
				optionfilters |= OP_TRANSLITERATOR;
				i++;
			}
		}
	}
	
	
	if (runquery == (RQ_BOOK | RQ_REF)) {
 	    doquery(maxverses, outputformat, outputencoding, optionfilters, searchtype, range, text, locale, ref, &cout, script, variants);
	}
	else printsyntax();

	return 0;
}
