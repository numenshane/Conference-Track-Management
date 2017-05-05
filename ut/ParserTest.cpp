#include "ParserTest.h"
#define PARSER_FILENAME "ParserTest.txt"
#define PARSER_RESULT_FILENAME "ParserTestResult.txt"

#include <iostream>
#include <fstream>
#include <cassert>

ParserTest::ParserTest()
{
    m_parser = NULL;
}
ParserTest::~ParserTest()
{
}

void ParserTest::setUp()
{
    if( m_parser == NULL)
        m_parser = new TalkParser();
}

void ParserTest::tearDown()
{
    if(m_parser != NULL)
        delete m_parser;
    m_parser = NULL;
}

void ParserTest::testParse()
{
    m_parser->Parse(PARSER_FILENAME);
    vector<Talk> talks = m_parser->GetTalks();
    ifstream infile;
    infile.open(PARSER_RESULT_FILENAME);
    assert(infile.is_open());
    string s;
    for (vector<Talk>::iterator iter = talks.begin(); iter != talks.end(); iter++)
    {
        CPPUNIT_ASSERT( getline(infile,s));
        string line = (*iter).m_title;
        line += "....";
        line += to_string((*iter).m_mins);
        CPPUNIT_ASSERT_MESSAGE( "parse extracted failed", s == line);
    }
}
