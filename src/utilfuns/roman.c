/*
 * Roman v1.1 - roman numeral <-> decimal converter.
 * Copyright (c) 1997 Simon Kittle <tsw-lo@usa.net>
 * Copying:  GNU GPL v2.0 or at your choice a later version.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, 
 * MA  02111-1307, USA.
 * 
 * 
 * *******************************************************
 * *******************************************************
 * Compile:  gcc -o roman -Wall -pedantic -ansi -O roman.c
 * 
 * Notes:  for roman numerals like V^{bar} meaning 5000 the following 
 *         key is used:
 * 
 * I = 1      \
 * V = 5      |
 * X = 10     |
 * L = 50     |-- normal symbols
 * C = 100    |
 * D = 500    |
 * M = 1000   /
 * v = 5000      (should be V^bar)
 * x = 10000     (should be X^bar)
 * Y = 50000     (should be L^bar)
 * A = 100000    (should be C^bar)
 * W = 500000    (should be D^bar)
 * R = 1000000   (should be M^bar)
 * 
 * CHANGES v1.1 over v1.0 - tightened up to_rom function by
 *         using the array of pointers whic point to begining
 *         of arrays which hold symbols.
 *         Addded checking for illegal chars in from_rom function.
 * 
 * BUGS    I can't find any bugs with the actual converting routines,
 *         but it does not check very well for illegal input, esp. when
 *         going from Roman Numerals to Decimal
 *
 */

/*
Changes made 2001-02-25 by Chris Little <chrislit@crosswire.org> to code
for use in the SWORD Projct <http://www.crosswire.org/sword>
*/

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define VERSION "1.1"

/*
 * prototypes
 */
char* to_rom(int num, char *p);
int from_rom(char *s);
//void usage( char *argv);

/*
 * globals
 */
char *ones[] = {"\0","I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
char *tens[] = {"\0","X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
char *huns[] = {"\0","C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
char *thos[] = {"\0","M", "MM", "MMM", "Mv", "v", "vM", "vMM", "vMMM", "Mx"};
char *ttos[] = {"\0","x", "xx", "xxx", "xY", "Y", "Yx", "Yxx", "Yxxx", "xA"};
char *hhos[] = {"\0","A", "AA", "AAA", "AW", "W", "WA", "WAA", "WAAA", "AR"};
/* array of pointers which point to pointers above to make loops easy */
char **c_points[] = { ones, tens, huns, thos, ttos, hhos};

char symbs[] =  { 'I', 'V', 'X', 'L', 'C', 'D', 'M', 'v', 'x', 'Y', 'A', 'W', 'R'};

/* program begins */
/*
int main(int argc, char **argv)
{
   if( argc != 3)
     usage( *argv );
   if( !strcmp(*(argv+1), "--help") || !strcmp(*(argv+1), "--version") )
     usage( *argv );


   if( !strcmp(*(argv+1), "-d") )
     {  
	char string[40] = "";
	int dec;
	
	dec = atoi( *(argv+2) );
	if(dec >= 1000000)
	  {
	     printf("Illegal number, >= 1000000 !!\n");
	     exit(1);
	  }
	to_rom( dec, string );
	printf("%s\n", string);
	return(0);
     }
   
   if( !strcmp(*(argv+1), "-r") )
     {
	int decimal;
	decimal = from_rom( *(argv+2) );
	printf("%d\n", decimal);
     }
   
   return(0);
}
*/

/********  functions **************/

/*
void usage( char *argv )
{
   fprintf(stderr,"\nRoman v%s.  Copyright (c) 1997 Simon Kittle <tsw-lo@usa.net>\n", VERSION);
   fprintf(stderr,"Copying policy: GNU GPL v2.0 or later.  Compiled at " __TIME__" on " __DATE__ "\n\n");
   fprintf(stderr, "Usage: %s <option> <string/number>\n", argv );
   fprintf(stderr, "Options:\n");
   fprintf(stderr, " -d         decimal to roman (any number between 1 and 999999 inclusive)\n");
   fprintf(stderr, " -r         roman to decimal\n");
   fprintf(stderr, "\nFollowing key used for non displayable roman characters:\n");
   fprintf(stderr,"I = 1      \\\nV = 5      |\nX = 10     |\nL = 50     |-- normal symbols\nC = 100    |\nD = 500    |\nM = 1000   /\nv = 5000      (should be V^bar)\nx = 10000     (should be X^bar) \nY = 50000     (should be L^bar)\nA = 100000    (should be C^bar)\nW = 500000    (should be D^bar)\nR = 1000000   (should be M^bar)\n\n");
   exit(1);
}
*/

/*
 * decimal TO roman function
 */
char* to_rom(int dec, char *p)
{
   int one = dec , ten[6], c;
   
   for(c = 0; c < 6; c++)
     {
	/* code beings */
	ten[c] = one % 10;
	one /= 10;
     }
   for( c = 5; c >=0; c--)
	strcat( p, *( c_points[c] + ten[c] ) );

   return p;
}
/******************************************************/
int from_rom(char *s)
{
   int ones_n[] = {0,1,2,3,4,5,6,7,8,9};
   int tens_n[] = {0,10,20,30,40,50,60,70,80,90};
   int huns_n[] = {0,100,200,300,400,500,600,700,800,900};
   int thos_n[] = {0,1000,2000,3000,4000,5000,6000,7000,8000,9000};
   int ttos_n[] = {0,10000,20000,30000,40000,50000,60000,70000,80000,90000};
   int hhos_n[] = {0,100000,200000,300000,400000,500000,600000,700000,800000,900000};
   /* an array (set of pointers) of pointers that point to more pointers :-)*/
   int *i_points[] = {  ones_n, tens_n, huns_n, thos_n, ttos_n, hhos_n };
   
   
   int c1,c2,c3, count, len,decimal = 0, y = 0;
   char *p1, *p2 = NULL , *p3 = NULL , last = '\0', holding[5];
   
   p1 = s;
   len = strlen(s);

   /* check for one type of illegal string */
   for( count = 0; count < len; count++)
     {
	int uu, flag1 = 1;
	static int y = 0;
	if( last == *s) y++;
	else y = 0;
	if( y == 3)
	  {
	    return -1;
	    /*
	     fprintf(stderr,"Illegal Roman Numeral string!\n");
	     exit(1);
	    */
	  }
	/* check for illegal characters aswell */
	for( uu = 0; uu < 13; uu++)
	  {
	     if( (holding[0] = *s) == symbs[uu] )
	       flag1 = 0;
	  }
	if( flag1 == 1)
	  {
	    return -1;
	    /*
	     printf("Illegal character in roman string!\n");
	     exit(1);
	    */
	  }
	last = *s++;
     }
   
   s = p1 + (len - 1);  /* s points to very last character in roman string */
   for(c2 = 0; c2 < 5; c2++)
     holding[c2] = '\0';
   
   for(c3 = 0; c3 < 6; c3++)
     {
	p3 = s;
	for(c1 = 0; c1 < 4; c1++)
	  {
/*	     printf("%s\n", holding);
*/	     strncpy(holding, (p1 < s) ? s-- : p1, c1+1 );
	     for(count = 0; count < 10; count++)
	       {
		  if( !strcmp( holding,  *( c_points[ c3 ] + count) )  )
		    {
		       /* i_points holds an array of pointers which each point
			* to one more array, units, tens, hundreds, etc
			* c_points above is same but points to not integers but to
			* the roman equivalent */
		       y = *( i_points[ c3 ] + count) ;
		       p2 = s;
		    }
		  else
		    {
		       /* this is to cope with fact may be no ones, or no
			* tens etc.  have to set pointer right back
			*/
		       if( y == 0)
			    p2 = p3;
		    }
	       }
	  }
	decimal += y;
	s = p2;
	for(c2 = 0; c2 < 5; c2++)
	  holding[c2] = '\0';
	y = 0;     
     }
   return( decimal );
}
