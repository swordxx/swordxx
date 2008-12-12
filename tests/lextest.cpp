#include <iostream>
#include <rawld.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	RawLD::createModule("tmp/lextest");
	RawLD lex("tmp/lextest");

	lex.setKey("b");
	lex << "entry for b";

	lex.setKey("a");
	lex << "entry for a";

	lex = TOP;
	cout << lex.getKeyText() << endl;
	lex++;
	cout << lex.getKeyText() << endl;

	lex.setKey("a");
	lex.deleteEntry();

//	lex.setKey("a");
//	lex << "y";

	lex = BOTTOM;
	return 0;
}
