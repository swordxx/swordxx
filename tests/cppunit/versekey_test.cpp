#include <cppunit/extensions/HelperMacros.h>

#include <iostream>

#include "swbuf.h"
#include "versekey.h"

using namespace sword;

class VerseKeyTest : public CppUnit::TestFixture  {
CPPUNIT_TEST_SUITE( VerseKeyTest );
CPPUNIT_TEST( testSingleKeyParsing );
CPPUNIT_TEST_SUITE_END();

private:
	sword::VerseKey* m_vk1;
	sword::VerseKey* m_vk2;
	sword::VerseKey* m_vk3;
	
protected:
	void setLocaleToAll(const char* name) {
		m_vk1->setLocale(name);
		m_vk2->setLocale(name);
		m_vk3->setLocale(name);
	};
	
	SWBuf parseKey(const char* keyValue, const char* locale) {
		sword::VerseKey vk;
		vk.setLocale(locale);
		vk.setText(keyValue);
				
		SWBuf ret( vk.getText() );
		//std::cout << ret.c_str();
		return ret;
	};
	
public:
	void setUp() {
		m_vk1 = new sword::VerseKey();
		m_vk2 = new sword::VerseKey();
		m_vk3 = new sword::VerseKey();
		
		setLocaleToAll("en");
	}	
	void tearDown()  {
		delete m_vk1;
		delete m_vk2;
		delete m_vk3;
	}
	
	void testSingleKeyParsing() {
		//testing with I John 2:3 and locale en
		CPPUNIT_ASSERT( parseKey("1jn.2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("1 jn.2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("Ijn.2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("I jn.2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "1jn 2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "1 jn 2.3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "Ijn 2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "I jn 2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "1jn.2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "1 jn.2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "Ijn.2:3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "I jn.2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "1jn 2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "1 jn 2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey( "Ijn 2:3", "en") 		== "I John 2:3");
		
		CPPUNIT_ASSERT( parseKey("1.jn.2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("1. jn.2.3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("I.jn.2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("I. jn.2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("1.jn 2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("1. jn 2.3", "en")		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("I.jn 2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("I. jn 2.3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("1.jn.2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("1. jn.2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("I.jn.2:3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("I. jn.2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("1.jn 2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("1. jn 2:3", "en") 	== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("I.jn 2:3", "en") 		== "I John 2:3");
		CPPUNIT_ASSERT( parseKey("I. jn 2:3", "en") 	== "I John 2:3");
		
	
		//testing the same with german locale
		CPPUNIT_ASSERT( parseKey("1jn.2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("1 jn.2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("Ijn.2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("I jn.2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "1jn 2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "1 jn 2.3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "Ijn 2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "I jn 2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "1jn.2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "1 jn.2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "Ijn.2:3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "I jn.2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "1jn 2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "1 jn 2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey( "Ijn 2:3", "de") 		== "1. Johannes 2:3");
		
		CPPUNIT_ASSERT( parseKey("1.jn.2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("1. jn.2.3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("I.jn.2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("I. jn.2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("1.jn 2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("1. jn 2.3", "de")		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("I.jn 2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("I. jn 2.3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("1.jn.2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("1. jn.2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("I.jn.2:3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("I. jn.2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("1.jn 2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("1. jn 2:3", "de") 	== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("I.jn 2:3", "de") 		== "1. Johannes 2:3");
		CPPUNIT_ASSERT( parseKey("I. jn 2:3", "de") 	== "1. Johannes 2:3");	
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(VerseKeyTest);
