#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <listkey.h>
#include <versekey.h>

int main(int argc, char **argv)
{
	ListKey lk, lk2;
	VerseKey vk("jn 1:1", "jn 1:12");
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
	lk2 << "test2";
	for (lk2 = TOP; !lk2.Error(); lk2++)
		cout << (const char *) lk2 << "\n";

	return 0;
}
