#include <iostream>
#include <rawld.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv)
{
	RawLD::createModule("tmp/lextest");
	RawLD lex("tmp/lextest");

	lex.setKey("b");
	lex << "x";

	lex.setKey("a");
	lex << "x";

	lex.setKey("a");
	lex.deleteEntry();

//	lex.setKey("a");
//	lex << "y";

	lex = BOTTOM;
	return 0;
}
