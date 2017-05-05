#include <string>
using namespace std;
#define UNUSED(x) (void)x

#ifndef __TALK_H__
#define __TALK_H__

class Talk {
public:
    Talk(int mins, string title);
public:
    int m_mins;
    string m_title;
};
#endif
