#include <stdio.h>
#include <iostream>
#include <versekey2.h>
#include <stdlib.h>

#include <refsysmgr.h>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	cout << "Default refsys: " << 
		RefSysMgr::getSystemRefSysMgr()->getDefaultRefSysName() << endl;
	StringList tlist = RefSysMgr::getSystemRefSysMgr()->getAvailableRefSys();
	for (StringList::const_iterator it = tlist.begin(); it != tlist.end(); it++) {
		cout << (*it).c_str() << endl;
	}
	VerseKey2 *testkey;
	testkey = RefSysMgr::getSystemRefSysMgr()->getVerseKey("WEB", "Judith 1:1");
	//testkey = RefSysMgr::getSystemRefSysMgr()->getVerseKey("KJV", "John 3:16");
	if (testkey)
		cout << testkey->getText() << endl;
	else
		cout << "Failed to get key" << endl;
}
