#include <cppunit/extensions/HelperMacros.h>

#include <iostream>

#include "stringmgr.h"

using namespace sword;

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
		CPPUNIT_ASSERT( !strcmp(mgr->upperLatin1("hi this is a test"), "HI THIS IS A TEST") );
	}
	
	void testUpperUTF8() {
		StringMgr* mgr = StringMgr::getSystemStringMgr();
		if (mgr->hasUTF8Support()) {
			CPPUNIT_ASSERT( !strcmp(mgr->upperUTF8("äüöß"), "ÄÜÖß") );
		}	
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(StringMgrTest);
