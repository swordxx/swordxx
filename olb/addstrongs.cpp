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

#include "online.dcl"
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>

/*
   Online Bible Database Access Routines
*/

INT versedata[MAX_VERSE];
CHAR data[MAX_DATA];

void main(argc, argv)
int  argc;
CHAR *argv[];
{
	int fd;
 CHAR found, passage[64], *p;
 int i, j;
 INT word;
short verse_list[MAX_LIST];
 extern struct zonline online;

 if (argc > 1)
    call DOINIT(argv[1]);   /* initialise system */
 else
    call DOINIT("g:\\bible\\av");   /* initialise system */
	fd = open("../modules/texts/rawtext/kjv/ot", O_RDONLY, S_IREAD|S_IWRITE);

					   /* get text for Ge 1:1 & Rev 22:21 */

 for (i = 1; i < 8; i++)
    puts(GETVRSN(i));

 verse_list[0] = 10;
 for (i=1; i <= 8; i++)
    verse_list[i] = i;
 verse_list[9] = 31101;
 verse_list[10] = 31102;
 for (i = 1; i <= 10; i++)
    puts(VRSTOREF(verse_list[i], 0));
 call DUMPVRS(verse_list, versedata, data);

 word = MAPWORD("Aaron", &found);   /* map a word into an absolute number */
 call PUTLIST("Aaron=", word);
 if (found == 0) then
    puts("*Not Found");
 else
    puts("*Found");
 call GETWORD(word, verse_list);    /* get list of verses for a word */
 call DUMPVRS(verse_list, versedata, (CHAR *) data);

 /* first word in index */
 word = MAPWORD("a", &found);   /* map a word into an absolute number */
 call PUTLIST("a=", word);
 if (found == 0) then
    puts("*Not Found");
 else
    puts("*Found");
 call GETWORD(word, verse_list);    /* get list of verses for a word */
 call DUMPVRS(verse_list, versedata, (CHAR *) data);
 call GETSTRNG(1, verse_list, 0);   /* get list of verses for Strong's */
 call DUMPVRS(verse_list, versedata, (CHAR *) data);

 /* last word in index */
 word = MAPWORD("Zuzims", &found);   /* map a word into an absolute number */
 call PUTLIST("Zuzims=", word);
 if (found == 0) then
    puts("*Not Found");
 else
    puts("*Found");

 call GETWORD(word, verse_list);    /* get list of verses for a word */
 call DUMPVRS(verse_list, versedata, (CHAR *) data);

 printf("\nCross References for Ge 1:1\n");
 call GETCXREF(1, verse_list);      /* get cross-references for a verse */
 verse_list[1] = verse_list[66];
 verse_list[2] = verse_list[67];
 call DUMPVRS(verse_list, versedata, (CHAR *) data);

 for (i = 1; i <= 31102; i++)   /* verify conversion routines work */
    {
	if ((i % 1000) == 0) then
        puts(DECIMAL(i));
	p = VRSTOREF(i, 0);
	if (*p < '9' && *(p+1) == ' ') then
	   {                      /* fix book names like 1 Timothy -> 1Timothy */
         *(p+1) = *p;
	    *p = ' ';
	   }
	j = REFTOVRS(p);
     if (j != i) then
        {
	    printf("i=%d, j=%d, ref=%s\n", i, j, p);
	    call ERROR("VRSTOREF - Error");
        }
    }

 DOEND();
}
