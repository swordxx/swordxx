#include <iostream>
#include <utilstr.h>
#include <swbuf.h>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {
	const char *buf = (argc > 1) ? argv[1] : "Description=German Unrevidierte Luther Übersetzung von 1545";

	SWBuf fixed = assureValidUTF8(buf);

	cout << "input / processed:\n" << buf << "\n" << fixed << endl;

	return 0;
}
