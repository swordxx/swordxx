#include <stdio.h>
#include <versekey.h>

using namespace sword;

int
main(int argc, char *argv[])
{
	int i;
	long offset1, offset2, otoffset;
	int *vmaxarray;
	int vmax;
	sword::VerseKey *tk = new sword::VerseKey("Genesis 0:0");
	
	//tk->Testament(1);
	//tk->Book(1);
	//tk->Chapter(0);
	//tk->Verse(0);
	//printf("bcv %d %d:%d\n", tk->Book(), tk->Chapter(), tk->Verse());
	printf("{0, 0}, // OT Header\n");
	while (tk->Testament() == 1)
	{
		offset1 = tk->otbks[tk->Book()];
		if (tk->Chapter() == 1) {
			offset2 = tk->otcps[(int)offset1];
			printf("{%d, 0}, // %s:0\n", offset2, tk->getBookName());
		}
		offset2 = tk->otcps[(int)offset1 + tk->Chapter()];
		vmaxarray = tk->builtin_books[tk->Testament()-1][tk->Book()-1].versemax;
		vmax = vmaxarray[tk->Chapter()-1];

		printf("{%d, %d}, // %s:%d\n", offset2, vmax, tk->getBookName(), tk->Chapter());
		tk->Chapter(tk->Chapter()+1);
		otoffset = offset2+vmax+1;
	}
	printf("{%d, 0}, // NT Header\n", otoffset);
	while (!tk->Error())
	{
		offset1 = tk->ntbks[tk->Book()];
		if (tk->Chapter() == 1) {
			offset2 = tk->ntcps[(int)offset1]+otoffset;
			printf("{%d, 0}, // %s:0\n", offset2, tk->getBookName());
		}
		offset2 = tk->ntcps[(int)offset1 + tk->Chapter()] + otoffset;
		vmaxarray = tk->builtin_books[tk->Testament()-1][tk->Book()-1].versemax;
		vmax = vmaxarray[tk->Chapter()-1];

		printf("{%d, %d}, // %s:%d\n", offset2, vmax, tk->getBookName(), tk->Chapter());
		tk->Chapter(tk->Chapter()+1);
	}
	delete tk;
  return 0;
}
