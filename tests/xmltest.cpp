#include <utilxml.h>
#include <iostream>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {
	XMLTag x((argc > 1) ? argv[1] : "<verse osisID=\"John.1.1\" type=\"test type\" yeah = \"stuff\" />");
//	x.setAttribute("newOne", "oneValue");
	cout << x.toString() << "\n";
	cout << "Tag name: [" << x.getName() << "]\n";
	ListString attributes = x.getAttributeNames();
	for (ListString::iterator it = attributes.begin(); it != attributes.end(); it++) {
		const char *name = it->c_str();
		cout << " - attribute: [" << name << "] = [";
		cout << x.getAttribute(name) << "]\n";
	}
	cout << " isEmpty: " << x.isEmpty() << "\n";
	cout << " isEndTag: " << x.isEndTag() << "\n";
	cout << "\n";
}
