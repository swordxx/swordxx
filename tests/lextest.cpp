#include <rawld.h>

void main(int argc, char **argv) {

	RawLD::createModule("tmp/lextest");
	RawLD lex("tmp/lextest");

	lex.SetKey("a");
	lex << "aaa";

	lex.SetKey("c");
	lex << "ccc";

	lex.SetKey("b");
	lex << "bbb";

	for (lex = TOP; !lex.Error(); lex++) {
		cout << lex.KeyText() << ":\n";
		cout << lex << "\n------------------\n";
	}
	return 0;
}
