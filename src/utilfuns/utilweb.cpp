#include <utilxml.h>
#include <stdio.h>
#include <map>

SWORD_NAMESPACE_START

using std::map;

typedef map<unsigned char,SWBuf> DataMap;

const SWBuf encodeURL( const SWBuf& url ) {
    	DataMap m;
	for (unsigned short int c = 32; c <= 255; ++c) { //first set all encoding chars
			if ( (c>='A' && c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9') || strchr("-_.!~*'()", c)) {
				continue; //we don't need an encoding for this char
			}

			char s[5];
			sprintf(s, "%-.2X", c); //left-aligned, 2 digits, uppercase hex
			m[c] = SWBuf("%") + s; //encoded char is "% + 2 digit hex code of char"
	}
	//the special encodings for certain chars
	m[' '] = '+';

	SWBuf buf;
	const int length = url.length();
	for (int i = 0; i <= length; i++) { //fill "buf"
		const char& c = url[i];
		buf += (!m[c].length()) ? (SWBuf)c : (SWBuf)m[c];
	}

	return buf;
}

SWORD_NAMESPACE_END
