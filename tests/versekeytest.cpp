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

     const char *bounds = "lk,acts";
     scope = parser.ParseVerseList(bounds, parser, true);

     VerseKey boundTest("lk", "acts");

     boundTest.setText("Is.1.13");
     std::cout << "Error: " << (int)boundTest.Error() << "\n";
     boundTest.setText("1Sam.21.1");
     std::cout << "Error: " << (int)boundTest.Error() << "\n";


     x = "Is.1.13";
     scope = x;
//     if (scope == x) std::cout << "Error restricting bounds: " << x.getText() << " is in " << bounds << "\n";
     if (!scope.Error()) std::cout << "Error restricting bounds: " << x.getText() << " is in " << bounds << "\n";

     x = "1Sam.21.1";
     scope = x;
     if (!scope.Error()) std::cout << "Error restricting bounds: " << x.getText() << " is in " << bounds << "\n";

	return 0;
}
