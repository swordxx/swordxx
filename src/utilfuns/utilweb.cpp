#include <utilxml.h>
#include <stdio.h>
#include <string>
#include <map>

SWORD_NAMESPACE_START

using std::string;
using std::map;

typedef map<unsigned char,string> DataMap;

const std::string encodeURL( const std::string& url ) {
    	DataMap m;
	for (unsigned short int c = 32; c <= 255; ++c) { //first set all encoding chars
			if ( (c>='A' && c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9') || strchr("-_.!~*'()", c)) {
				continue; //we don't need an encoding for this char
			}

			char s[5];
			sprintf(s, "%-.2X", c); //left-aligned, 2 digits, uppercase hex
			m[c] = string("%") + s; //encoded char is "% + 2 digit hex code of char"
	}
	//the special encodings for certain chars
	m[' '] = '+';

	string buf;
	const int length = url.length();
	for (int i = 0; i <= length; i++) { //fill "buf"
		const char& c = url[i];
		buf += m[c].empty() ? string(1,c) : m[c];
	}

	return buf;
}

SWORD_NAMESPACE_END
