#include <iostream>
#include <rawld.h>

void main(int argc, char **argv)
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
}
