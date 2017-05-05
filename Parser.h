#include "Talk.h"
#include <vector>

#ifndef __PARSER_H__
#define __PARSER_H__

class Parser {
public:
    virtual ~Parser() {}
    virtual void Parse(string fileName)=0;
    vector<string> GetContents() {return m_contents;}
protected:
    vector<string> m_contents;
};

class TalkParser: public Parser{
public:
    void Parse(string fileName);
    vector<Talk> GetTalks();
private:
   int extractTimeFromTalk(string talkName);
private:
    vector<Talk> m_talks; 
};
#endif
