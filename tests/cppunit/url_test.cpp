#include <cppunit/extensions/HelperMacros.h>

#include <iostream>

#include "url.h"

using namespace sword;

class URLTest : public CppUnit::TestFixture  {
CPPUNIT_TEST_SUITE( URLTest );
CPPUNIT_TEST( testProtocol );
CPPUNIT_TEST( testHostName );
CPPUNIT_TEST( testPath );
CPPUNIT_TEST( testParametersMap );
CPPUNIT_TEST( testParameterValue );
CPPUNIT_TEST( testEncode );
CPPUNIT_TEST( testDecode );
CPPUNIT_TEST_SUITE_END();

private:
	sword::URL* m_url1;
	sword::URL* m_url2;
	sword::URL* m_url3;
	
public:
	void setUp() {
		m_url1 = new sword::URL("http://www.crosswire.org/index.jsp?page=help&user=foo&name=bar");
		m_url2 = new sword::URL("ftp://ftp.crosswire.org/sword/wiki/index.jsp?page=help&amp;user=foo&amp;name=foo%20bar");
		m_url3 = new sword::URL("crosswire.org/index.jsp");
	}	
	void tearDown()  {
		delete m_url1;
		delete m_url2;
		delete m_url3;
	}
	
	void testProtocol()
	{
		CPPUNIT_ASSERT( !strcmp(m_url1->getProtocol(), "http") );
		CPPUNIT_ASSERT( !strcmp(m_url2->getProtocol(), "ftp") );
		CPPUNIT_ASSERT( m_url3->getProtocol() && strlen( m_url3->getProtocol() ) == 0 );
	}
	
	void testHostName()
	{
		CPPUNIT_ASSERT( !strcmp(m_url1->getHostName(), "www.crosswire.org") );
		CPPUNIT_ASSERT( !strcmp(m_url2->getHostName(), "ftp.crosswire.org") );
		CPPUNIT_ASSERT( !strcmp(m_url3->getHostName(), "crosswire.org") );
	}

	void testPath()
	{
		CPPUNIT_ASSERT( !strcmp(m_url1->getPath(), "/index.jsp") );
		CPPUNIT_ASSERT( !strcmp(m_url2->getPath(), "/sword/wiki/index.jsp") );
		CPPUNIT_ASSERT( !strcmp(m_url3->getPath(), "/index.jsp") );
	}

	void testParametersMap()
	{	
 		std::map< sword::SWBuf, sword::SWBuf > params = m_url1->getParameters();
		CPPUNIT_ASSERT( !strcmp(params[sword::SWBuf("page")].c_str(), "help") );
		CPPUNIT_ASSERT( !strcmp(params[sword::SWBuf("user")].c_str(),  "foo") );
		CPPUNIT_ASSERT( !strcmp(params[sword::SWBuf("name")].c_str(), "bar") );
	
 		params = m_url2->getParameters(); //test url2 params
		CPPUNIT_ASSERT( !strcmp(params[sword::SWBuf("page")].c_str(), "help") );
		CPPUNIT_ASSERT( !strcmp(params[sword::SWBuf("user")].c_str(),  "foo") );
		CPPUNIT_ASSERT( !strcmp(params[sword::SWBuf("name")].c_str(), "foo bar") );
	
 		params = m_url3->getParameters(); //test url3 params
		CPPUNIT_ASSERT( params.size() == 0 );
	}
	
	void testParameterValue()
	{	
 		CPPUNIT_ASSERT( !strcmp(m_url1->getParamterValue("page"), "help") );
		CPPUNIT_ASSERT( !strcmp(m_url1->getParamterValue("user"), "foo") );
		CPPUNIT_ASSERT( !strcmp(m_url1->getParamterValue("name"), "bar") );
		
		CPPUNIT_ASSERT( !strcmp(m_url2->getParamterValue("page"), "help") );
		CPPUNIT_ASSERT( !strcmp(m_url2->getParamterValue("user"), "foo") );
		CPPUNIT_ASSERT( !strcmp(m_url2->getParamterValue("name"), "foo bar") );
		
		CPPUNIT_ASSERT( m_url3->getParamterValue("page") && strlen(m_url3->getParamterValue("page")) == 0 );
	}	

	void testEncode() {	
		const char* encoded = URL::encode("this is a test");
		CPPUNIT_ASSERT( !strcmp(encoded, "this%20is%20a%20test") || !strcmp(encoded, "this+is+a+test") );
		
		CPPUNIT_ASSERT( !strcmp(URL::encode("this-is-a-test"), "this-is-a-test") );
		CPPUNIT_ASSERT( !strcmp(URL::encode(""), "") );
	}
	
	void testDecode() {
		CPPUNIT_ASSERT( !strcmp(URL::decode("this%3Eis%3Ea%3Etest"), "this>is>a>test") );
		CPPUNIT_ASSERT( !strcmp(URL::decode("this%3Eis%3Ea%3Etest%3E"), "this>is>a>test>") );
		CPPUNIT_ASSERT( !strcmp(URL::decode("%3E%3E%3E%3E%3E%3E%3E%3E%3E%3E%20%20%20%20%20"), ">>>>>>>>>>     ") );
		CPPUNIT_ASSERT( !strcmp(URL::decode("nothing%20"), "nothing ") );
		CPPUNIT_ASSERT( !strcmp(URL::decode("nothing"), "nothing") );
		CPPUNIT_ASSERT( !strcmp(URL::decode(""), "") );
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(URLTest);
