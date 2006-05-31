#include <utilxml.h>
#include <iostream>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {
	const char *xml = "<verse osisID=\"John.1.1\" type=\'test type\' yeah = \"stuff\" />";
	cout << ((argc > 1) ? argv[1]: xml) << "\n";
	XMLTag x((argc > 1) ? argv[1] : xml);
//	x.setAttribute("newOne", "oneValue");
	cout << x.toString() << "\n";
	x.setAttribute("addedAttribute", "with a \" quote");
	cout << x.toString() << "\n";
	cout << "Tag name: [" << x.getName() << "]\n";
	StringList attributes = x.getAttributeNames();
	for (StringList::iterator it = attributes.begin(); it != attributes.end(); it++) {
		const char *name = it->c_str();
		cout << " - attribute: [" << name << "] = [";
		cout << x.getAttribute(name) << "]\n";
	}
	cout << " isEmpty: " << x.isEmpty() << "\n";
	cout << " isEndTag: " << x.isEndTag() << "\n";
	cout << "\n";
}
