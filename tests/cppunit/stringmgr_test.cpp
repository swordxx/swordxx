#include <cppunit/extensions/HelperMacros.h>

#include <iostream>

#include "stringmgr.h"

using namespace sword;
using namespace std;

class StringMgrTest : public CppUnit::TestFixture  {
CPPUNIT_TEST_SUITE( StringMgrTest );
CPPUNIT_TEST( testUpperLatin1 );
CPPUNIT_TEST( testUpperUTF8 );
CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {
	}	
	void tearDown()  {
	}
	
	void testUpperLatin1() {	
		StringMgr* mgr = StringMgr::getSystemStringMgr();
		
		CPPUNIT_ASSERT( !strcmp(mgr->upperLatin1(""), "") );
		
		char t[10] = "Test!";
		const char* ret = mgr->upperLatin1(t);
		cout << ret << endl;
		CPPUNIT_ASSERT( ret && !strcmp(ret, "TEST!") );
	}
	
	void testUpperUTF8() {
		StringMgr* mgr = StringMgr::getSystemStringMgr();
		if (mgr->hasUTF8Support()) {
			char t[10] = "hi";
			char* ret = mgr->upperUTF8(t);
			CPPUNIT_ASSERT( ret && !strcmp(ret, "HI") );
		}	
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(StringMgrTest);
