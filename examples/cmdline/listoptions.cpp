#include <iostream>
#include <swmgr.h>

using sword::SWMgr;
using sword::StringList;
using std::cout;

int main(int argc, char **argv)
{
	SWMgr library;

	StringList options = library.getGlobalOptions();
	for (StringList::const_iterator it = options.begin(); it != options.end(); it++) {
		cout << *it << " (" << library.getGlobalOptionTip(*it) << ")\n";
		StringList optionValues = library.getGlobalOptionValues(*it);
		for (StringList::const_iterator it2 = optionValues.begin(); it2 != optionValues.end(); it2++) {
			cout << "\t" << *it2 << "\n";
		}
	}
	return 0;
}
