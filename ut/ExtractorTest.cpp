#include "ExtractorTest.h"
#include "../Parser.h"
#define SUC_EXTRACT_FILENAME "SucExtractTest.txt"
#define FAIL_EXTRACT_FILENAME "FailExtractTest.txt"
//#define SUC_EXTRACT_RESULT_FILENAME "SucExtractTestResult.txt"
//#define FAIL_EXTRACT_RESULT_FILENAME "FailExtractTestResult.txt"

#include <iostream>
#include <fstream>
#include <cassert>

ExtractorTest::ExtractorTest()
{
    m_extractor = NULL;
}
ExtractorTest::~ExtractorTest()
{
    
}

void ExtractorTest::setUp()
{
    
}

void ExtractorTest::tearDown()
{
    
}

void ExtractorTest::testExtractSuc()
{
    TalkParser parser;
    parser.Parse( SUC_EXTRACT_FILENAME );
    vector<Talk> talks = parser.GetTalks(); 
    if( !talks.empty() && m_extractor == NULL)
        m_extractor = new TwoSessionTrackExtractor(talks, 3*60, 3*60, 3*60, 4*60);
    CPPUNIT_ASSERT( m_extractor->Extract());
    //SequenceVisitor visitor;
    //visitor.visit(m_extractor);
    delete m_extractor; 
    m_extractor = NULL;   
}

void ExtractorTest::testExtractFail()
{   
    TalkParser parser;
    parser.Parse( FAIL_EXTRACT_FILENAME );
    vector<Talk> talks = parser.GetTalks();
    if( !talks.empty() && m_extractor == NULL)
        m_extractor = new TwoSessionTrackExtractor(talks, 3*60, 3*60, 3*60, 4*60);
    CPPUNIT_ASSERT( !m_extractor->Extract());
    delete m_extractor;
    m_extractor = NULL;   
}
