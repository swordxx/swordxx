#include <swbuf.h>
using namespace sword;

//#include <string>
//#define SWBuf string

#include <iostream>
using namespace std;

#define BASEI 32000000L

SWBuf speed;

void appendChTest() {
	cerr << "\nappend ch test -------\n";
	cerr.flush();
	for (unsigned long i = 0; i < BASEI+14; i++) {
		speed += (char) (i%125)+1;
	}
	speed.resize(BASEI/100L);
	cout << "\nappend ch test -------\n";
	cout << speed << "\n";
}


void appendStringTest() {
	// append test
	cerr << "\nappend string test -------\n";
	cerr.flush();
	unsigned long iterations = BASEI/2L;
	for (unsigned long i = 0; i < iterations; i++) {
		speed.append("this is a test", 3);
		if (!(i%3))speed.append("test");
	}
	cout << "\nappend string test -------\n";
	speed = SWBuf(speed.c_str() + iterations);
	speed.resize(BASEI/100L);
	cout << speed << "\n";
}


void subscriptTest() {
	// subscript access test;
	cerr << "\nsubscript access test -------\n";
	cerr.flush();
	for (int j = 0; j < 200; j++) {
		for (unsigned long i = speed.length()-1; i; i--) {
			speed[i] = (char) (i%40)+65;
		}
	}
	cout << "\nsubscript access test -------\n";
	cout << speed << "\n";
}

void ctorAssignTest() {
	// construct and assign test
	cerr << "\nconstructor and assign test -------\n";
	cerr.flush();
	for (unsigned long i = (BASEI/8000L); i; i--) {
		SWBuf speedl;
		speedl = speed;
	}
	cout << "\nconstructor and assign test -------\n";
	SWBuf speedl;
	speedl = speed;
	cout << speedl << "\n";
}

void compareTest() {
	// compare test
	cerr << "\ncompare test -------\n";
	cerr.flush();
	SWBuf first = "firsttest";
	SWBuf second = "first2test";
	cout << "\ncompare test -------\n";
	for (unsigned long i = BASEI/5; i; i--) {
		if (first != second)
			if (first >= second)
				if (first < second);
				else if (!(BASEI%1000)) cout << "g";
	}
}



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
	cout << "should be (hello wurld hello wurld): (" << y << ")\n";
//	y.appendFormatted(" from %d %s running %02.05f miles", 4, "dogs", 1.9f);
	cout << "should be (hello wurld hello wurld from 4 dogs running 1.90000 miles): (" << y << ")\n";
	y += '!';
	cout << "should be (hello wurld hello wurld from 4 dogs running 1.90000 miles!): (" << y << ")\n";
	y.append(y.c_str(),5);
	cout << "should be (hello wurld hello wurld from 4 dogs running 1.90000 miles!hello): (" << y << ")\n";

	appendChTest();
	appendStringTest();
	subscriptTest();
	ctorAssignTest();
	compareTest();
		
}

