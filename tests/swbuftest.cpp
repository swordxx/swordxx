#include <swbuf.h>
#include <iostream>

using namespace std;
using namespace sword;

int main(int argc, char **argv) {
	SWBuf x;
	cout << "x should be (): (" << x << ")\n";
	cout << "size should be 0: " << x.size() << "\n";
	x = "hello";
	cout << "x should be (hello): (" << x << ")\n";
	x += " world";
	cout << "x should be (hello world): (" << x << ")\n";
	cout << "size should be 11: " << x.size() << "\n";
	cout << "x[7] should be 'o': '" << x[7] << "'\n";
	x[7] = 'u';
	cout << "x[7] should be 'u': '" << x[7] << "'\n";
	cout << "x should be (hello wurld): (" << x << ")\n";
	SWBuf y = x + " " + x;
	cout << "should be (hello wurld hello world): (" << y << ")\n";
	y.appendFormatted(" from %d %s running %02.05f miles", 4, "dogs", 1.9f);
	cout << "should be (hello wurld hello world from 4 dogs running 1.90000 miles): (" << y << ")\n";
	y += '!';
	cout << "should be (hello wurld hello world from 4 dogs running 1.90000 miles!): (" << y << ")\n";
}

