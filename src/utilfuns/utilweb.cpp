
#include <utilxml.h>
#include <ctype.h>
#include <string>
#include <map>

SWORD_NAMESPACE_START

using std::string;
using std::map;
using std::pair;

typedef pair<string,string> DataPair;
typedef map<string,string> DataMap;

const std::string encodeURL( const std::string& url ) {
	string buf( url );

    DataMap m;
	m[" "] = "+";
	m[":"] = "%3A";

	DataMap::iterator it;

	for (it = m.begin(); it != m.end(); ++it) {
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
