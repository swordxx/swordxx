#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <listkey.h>
#include <versekey.h>


int main(int argc, char **argv)
{
	ListKey lk, lk2;
	VerseKey vk("jn 1:1", "jn 1:12");
	VerseKey vk2("jude", "jude");
	SWKey text;

	vk = "jas 1:19";
	text = (const char *)vk;
	lk << text;
	lk << text;
	lk << text;
	lk << "James 1:19";
	lk << "yoyo";
	lk << vk;
	lk2 << "test1";
	lk2 << lk;
	lk2 << vk2;
	lk2 << "test2";
	for (lk2 = TOP; !lk2.Error(); lk2++)
		cout << (const char *) lk2 << "\n";


	lk2 = VerseKey().ParseVerseList("mat-john", 0, true);

	VerseKey yoyo("john");
	yoyo = MAXCHAPTER;
	cout << yoyo;
/*

	for (int i = 0; i < 2; i++) {
	VerseKey x, y;
	ListKey lk3;
	x = "rev";
	y = x;
	x = "mat";
	VerseKey newElement;
	newElement.LowerBound(x);
	newElement.UpperBound(y);
	lk3 << newElement;

	lk2 << lk3;
	}

*/
	cout << "---------\n";

	for (lk2 = TOP; !lk2.Error(); lk2++)
		cout << (const char *) lk2 << "\n";

	return 0;
}
