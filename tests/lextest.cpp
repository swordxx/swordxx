#include <rawld4.h>

void main(int argc, char **argv) {

	RawLD4::createModule("tmp/lextest");
	RawLD4 lex("tmp/lextest");

	lex.SetKey("a");
	lex << "aaa";

	lex.SetKey("c");
	lex << "ccc";

	lex.SetKey("b");
	lex << "bbb";

	lex.SetKey("d");
	lex << "ddd";

	for (lex = BOTTOM; !lex.Error(); lex--) {
		cout << lex.KeyText() << ":\n";
		cout << lex << "\n------------------\n";
	}
	return 0;
}
