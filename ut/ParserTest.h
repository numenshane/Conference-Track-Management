#ifndef __PARSER_TEST_CASE__
#define __PARSER_TEST_CASE__
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../Parser.h"

class ParserTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ParserTest);
    CPPUNIT_TEST(testParse);
    CPPUNIT_TEST_SUITE_END();
public:
    ParserTest();
    ~ParserTest();
public:
    void setUp();
    void tearDown();
protected:
    void testParse();
private:
    TalkParser *m_parser;
};

#endif
