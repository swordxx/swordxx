extern "C" {
#include "online.dcl"
}

void main(int argc, char **argv)
{
	CHAR found, passage[64], *p;
	int i, j;
	INT word;
	short verse_list[MAX_LIST];
	short versedata[MAX_VERSE];
	char data[MAX_DATA];
	extern struct zonline online;
	int versenum = 0;

	
	if (argc > 1)
		call DOINIT(argv[1]);   /* initialise system */
	else	call DOINIT("g:\\bible\\av");   /* initialise system */

	for (versenum = 23146; versenum <= 23147 /*31102*/; versenum++) {
		call GETEXT(versenum, versedata, 1, 1, 1, 1);
		call BLDVERSE(versedata, data);
		printf("%s\n", VRSTOREF(versenum, 1));
//		printf("%s\n", data);
	}

	DOEND();
}