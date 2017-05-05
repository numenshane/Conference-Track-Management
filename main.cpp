#include <iostream>
#include <algorithm>
#include "Parser.h"
#include "Extractor.h"

int main()
{
    string fileName;
    cout << "Please input conferenceFileNamewithPath" << endl;
    cin >> fileName;
    //fileName = "conference.txt";
    
    // Parser part
    TalkParser parser;
    parser.Parse(fileName);
    vector<Talk> talks = parser.GetTalks();
    #ifdef DEBUG
        for (vector<Talk>::iterator iter= talks.begin(); iter!=talks.end(); iter++)
        {
            cout << (*iter).m_title << "...." << (*iter).m_mins << endl;
        }
        cout << "------------------------------" << endl;
    #endif
    
    // Extractor part
    TwoSessionTrackExtractor extractor(talks, 3*60, 3*60, 3*60, 4*60);
    if (extractor.Extract()) 
    {
        cout << "SUC: Conference arrangement succeed!" << endl;
        SequenceVisitor visitor;
        visitor.visit(&extractor);
    }
    else 
    {
        cout << "Error: Conference arrangement failed!" << endl;
        #ifdef DEBUG
            SequenceVisitor visitor;
            visitor.visit(&extractor);
        #endif
    }
}
