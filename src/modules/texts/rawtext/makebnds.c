#include <stdio.h>
#include <fcntl.h>


char *bnames[] = {
	"Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy",
	"Joshua", "Judges", "Ruth", "I Samual", "II Samuel",
	"I Kings", "II Kings", "I Chronicles", "II Chronicles", "Ezra",
	"Nehemiah", "Esther", "Job", "Psalms", "Proverbs",
	"Ecclesiastes", "Song of Solomon", "Isaiah", "Jeremiah", "Lamentations",
	"Ezekiel", "Daniel", "Hosea", "Joel", "Amos",
	"Obadiah", "Jonah", "Micah", "Nahum", "Habakkuk",
	"Zephaniah", "Haggai", "Zechariah", "Malachi",
	"Matthew", "Mark", "Luke", "John", "Acts",
	"Romans", "I Corinthians", "II Corinthians", "Galatians", "Ephesians",
	"Philippians", "Colossians", "I Thessalonians", "II Thessalonians", "I Timothy",
	"II Timothy", "Titus", "Philemon", "Hebrews", "James",
	"I Peter", "II Peter", "I John", "II John", "III John",
	"Jude", "Revelation of John"};



main(int argc, char **argv)
{
	int fp, vfp, cfp, bfp;
	long pos;
	int num1, num2, offset, offset2, chapmax, chapoff, chapoff2, curbook = 0, curchap = 0, curverse = 0;
	char buf[127];

	if (argc > 3) {
		fprintf(stderr, "usage: %s <file to process> [NT?]\n", argv[0]);
		exit(1);
	}

	if (argc > 2)
		curbook = 39;
	sprintf(buf, "%s.vss", argv[1]);
	if ((vfp = open(buf, O_RDONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.cps", argv[1]);
	if ((cfp = open(buf, O_RDONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.bks", argv[1]);
	if ((bfp = open(buf, O_RDONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	read(bfp, &offset2, sizeof(offset2));
	read(cfp, &chapoff2, sizeof(chapoff2));
	while (read(bfp, &offset, sizeof(offset)) == sizeof(offset)) {
		chapmax = (offset - offset2) / sizeof(offset);
		printf("\n\{\"%s\", %d}, \n// %s\n", bnames[curbook], chapmax, bnames[curbook]);
		curbook++;
		for (curchap = 0; curchap < chapmax; curchap++) {
			read(cfp, &chapoff, sizeof(chapoff));
			printf("%d, ", (chapoff - chapoff2) / sizeof(chapoff));
			chapoff2 = chapoff;
		}
		offset2 = offset;
	}
	pos     =       lseek(cfp, 0, SEEK_CUR);
	offset  = (int) lseek(cfp, 0, SEEK_END);
	chapmax = (offset - offset2) / sizeof(offset);
	printf("\n\{\"%s\", %d}, \n// %s\n", bnames[curbook], chapmax, bnames[curbook]);
	curbook++;
	lseek(cfp, pos, SEEK_SET);
	for (curchap = 0; curchap < chapmax - 1; curchap++) {
		read(cfp, &chapoff, sizeof(chapoff));
		printf("%d, ", (chapoff - chapoff2) / sizeof(chapoff));
		chapoff2 = chapoff;
	}
	chapoff = (int) lseek(vfp, 0, SEEK_END);
	printf("%d, ", (chapoff - chapoff2) / sizeof(chapoff));
	
	close(vfp);
	close(cfp);
	close(bfp);
	close(fp);
}
