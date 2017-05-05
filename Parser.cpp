#include "Parser.h"
#include <fstream>
#include <cassert>

void TalkParser::Parse(string fileName)
{
    m_contents.clear();
    // openFileforRead
    // for each line; 
        // add to m_contents
    ifstream infile;
    infile.open(fileName.data());
    assert(infile.is_open());
    string s;
    while(getline(infile,s))
    {
        m_contents.push_back(s);
    }
    infile.close();
}

int TalkParser::extractTimeFromTalk(string talkName)
{
    // iter to find Number characters then postfix with min or lightning
    const string postfix1 = "min";
    const string postfix2 = "lightning";
    size_t found = talkName.find_first_of("123456789");
    if (found != string::npos) 
    {
        int value = stoi(talkName.substr(found), &found);
        if ( talkName.substr(found).find(postfix1) != string::npos)
            return value;
        else if ( talkName.substr(found).find(postfix2) != string::npos)
            return value*5;
        else
            assert (0);
    }
    else
        //case: only lightning
        assert ( talkName.find(postfix2, talkName.length()-postfix2.length()) != string::npos);
        return 5;
}

vector<Talk> TalkParser::GetTalks()
{
    //loop contents
        //extractTimeFromTalk
        //add to m_talks
    //return m_talks;  
    for (vector<string>::iterator iter= m_contents.begin(); iter!= m_contents.end(); iter++)
    {
        int num = extractTimeFromTalk(*iter);
        Talk t(num, *iter);
        m_talks.push_back(t);
    }
    return m_talks;
}
