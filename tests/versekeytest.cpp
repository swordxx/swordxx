#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <versekey.h>
#include <listkey.h>
#include <localemgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	VerseKey parser;

     ListKey scope = parser.ParseVerseList("amos", parser, true);

	ListKey result;

	scope++;
	scope++;
	scope++;
	scope++;

	VerseKey x = scope;
	x.clearBound();

	std::cout << "x: " << x.getText() << "\n";

	result << x;

	std::cout << result.getText() << "\n";

	result = TOP;

	std::cout << result.getText() << "\n";

	return 0;
}
