
#include <utilxml.h>
#include <ctype.h>
#include <string>
#include <map>

SWORD_NAMESPACE_START

using std::string;
using std::map;
using std::pair;

typedef pair<string,string> DataPair;

const std::string encodeURL( const std::string& url ) {
	string buf( url );

        map<string, string> m;
	m[" "] = "+";
	m[":"] = "%3A";

	for (map<string,string>::iterator it = m.begin(); it != m.end(); ++it) {
		string search =  it->first;
		string replace = it->second;
		while (true) {
			string::size_type idx = buf.find(search);
			if (idx == string::npos)
				break;

			buf.replace(idx, search.length(), replace);
		}
	}

	return buf;
}

SWORD_NAMESPACE_END
