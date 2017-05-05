#ifndef __EXTRACTOR_TEST_CASE__
#define __EXTRACTOR_TEST_CASE__
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../Extractor.h"

class ExtractorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ExtractorTest);
    CPPUNIT_TEST(testExtractSuc);
    CPPUNIT_TEST(testExtractFail);
    CPPUNIT_TEST_SUITE_END();
public:
    ExtractorTest();
    ~ExtractorTest();
public:
    void setUp();
    void tearDown();
protected:
    void testExtractSuc();
    void testExtractFail();
private:
    TwoSessionTrackExtractor *m_extractor;
};

#endif
