#include "Extractor.h"
#include <cassert>
#include <algorithm>
#include <iostream>

// descend 
bool TalkCompare (Talk i, Talk j) { return (i.m_mins > j.m_mins); }

TrackExtractor::TrackExtractor(const Talks& talks)
{
    m_talks = talks;   
}

bool TrackExtractor::Extract(){ return true;}

void TrackExtractor::accpet(Visitor &v){ v.visit(this);}

bool TrackExtractor::combineTalks(Talks& talks, Talks& chosen_talks, int min_minutes, int max_minutes)
{
    // template descend sort
    sort(talks.begin(),talks.end(),TalkCompare);
    bool found = false;
    for (size_t i = 0 ;i < talks.size(); i++)
    {
        vector<int> index_arr;
        int total_mins = 0;
        for (size_t j = i; j < talks.size(); j++)
        {
            if ((total_mins + talks[j].m_mins) <= max_minutes)//account j
            {    
                index_arr.push_back(j);
                total_mins = total_mins + talks[j].m_mins;
                if (total_mins >= min_minutes) // basic qualified, but not fully qualified!
                {
                    found = true; 
					continue; // a little greedy
                }
            }
            else continue; //don't account j
        } 
        if(found == true)
        {
            //erase talk from talks then return erased talk vector
            while( !index_arr.empty() )
            {
                int index = index_arr.back();
                index_arr.pop_back();
                chosen_talks.push_back(talks[index]);
                talks.erase(talks.begin() + index);    
            }
            return true;
        }
    }
    return false;
}

void TrackExtractor::calculateTrackRange(int minTrackMinutes, int maxTrackMinutes)
{
    int totalMinutes=0;
    for(TalksIter iter = m_talks.begin(); iter != m_talks.end(); iter++)
    {
        totalMinutes += (*iter).m_mins;
    }
    m_trackRangeMax = totalMinutes/minTrackMinutes;
    m_trackRangeMin = totalMinutes/maxTrackMinutes;
    m_totalMinutes = totalMinutes;
}

string TrackExtractor::formatTime(int hour, int minutes)
{
    if ( hour < 0 || hour > 12 || minutes < 0 || minutes > 59 ) 
        return string("");
    string s_hour;
    string s_minutes;
    if ( hour < 10 )
        s_hour = "0" + to_string(hour);
    else
        s_hour = to_string(hour);
    if ( minutes < 10 )
        s_minutes = "0" + to_string(minutes);
    else
        s_minutes = to_string(minutes);
    
    return s_hour + ":" + s_minutes;
}

void TwoSessionTrackExtractor::accept(Visitor &v)
{
   v.visit(this); 
}

bool TwoSessionTrackExtractor::Extract()
{
    if ( !GetLimitedMorningSession(m_talks, m_morningSessions) ) 
    {
        #ifdef DEBUG
            cout << "Err: Combined morning sessions " << m_trackRangeMorningMin << " wanted, but actually has " <<  m_morningSessions.size() << endl;
        #endif
        return false;
    }
	int index = 0; // afternoonSession full capacity index
    index += GetMaxAfternoonSession(m_talks, m_afternoonSessions);
    // revised:　diff exactly matching from partial matching, store the excellent matching endpoint
    // for each sacrifice, supplement to afternoon from this matching endpoint to refactor by one morning session
    while ( m_morningSessions.size() > m_afternoonSessions.size()+1)
    {
        int round = m_morningSessions.size() - m_afternoonSessions.size();
        // sacrifice one last morningSessions, to rescue afternoonSessions
        Talks session = m_morningSessions.back();
        m_morningSessions.pop_back();
        copy(session.begin(), session.end(), back_inserter(m_talks));
        // reshuffling for afternoonSessions
        for (SessionsIter iter = m_afternoonSessions.begin() + index; iter != m_afternoonSessions.end(); iter++)
        {
            Talks session = *iter;
            copy(session.begin(), session.end(), back_inserter(m_talks)); 
        }
        index += GetMaxAfternoonSession(m_talks, m_afternoonSessions);
        if ( (round -1) == (int)(m_morningSessions.size() - m_afternoonSessions.size())) 
            //no progress
            return false;
    }
    if(!m_talks.empty())
    {
        int left_mins=0; 
        for (TalksIter iter = m_talks.begin(); iter != m_talks.end(); iter++)
            left_mins += iter->m_mins;
        if ( m_morningSessions.size() == m_afternoonSessions.size()+1 && left_mins <= m_afternoonSessionMax )
        {
            m_afternoonSessions.push_back(m_talks);
            return true;
        }
        if ( m_morningSessions.size() == m_afternoonSessions.size() && left_mins <= m_morningSessionMax )
        {
            m_morningSessions.push_back(m_talks);
            return true;
        }
        
        #ifdef DEBUG
            cout << "Warning: following talks can't be arranged!" << endl;
            for (TalksIter iter = m_talks.begin(); iter != m_talks.end(); iter++)
            {
                cout << "\t" << iter->m_title << endl;    
            }
        #endif
        return false;
    }
    return true;
}

TwoSessionTrackExtractor::TwoSessionTrackExtractor(const Talks& talks, int mornMin, int mornMax, int afterMin, int afterMax):TrackExtractor(talks)
{
    m_morningSessionMin = mornMin;
    m_morningSessionMax = mornMax;
    m_afternoonSessionMin = afterMin;
    m_afternoonSessionMax = afterMax;
    calculateTrackRange(mornMin+afterMin, mornMax+afterMax);
    //init from parent
    m_trackRangeMorningMin = m_trackRangeMin;
    m_trackRangeMorningMax = m_trackRangeMax;
    // two session for one track 
    if(m_totalMinutes% (mornMax+afterMax) > mornMax) m_trackRangeMorningMin+=1;
    if(m_totalMinutes% (mornMin+afterMin) > mornMax) m_trackRangeMorningMax+=1;
}

bool TwoSessionTrackExtractor::GetLimitedMorningSession(Talks& talks, Sessions& sessions)
{
    int i = 0 ;
    while(true)
    {
        Talks session;
        if ( combineTalks(talks, session, m_morningSessionMin, m_morningSessionMax) )
        {
            i++;
            sessions.push_back(session);
        }
        else break;
        if ( i >= m_trackRangeMorningMax ) return true;
    }
    if ( i < m_trackRangeMorningMin ) 
        return false; 
    return true;
}

int TwoSessionTrackExtractor::GetMaxAfternoonSession(Talks& talks, Sessions& sessions)
{
	int inc_full = 0;
    while(true)
    {
        Talks session;
        if ( combineTalks(talks, session, m_afternoonSessionMax, m_afternoonSessionMax) )
        {
            sessions.push_back(session);
			inc_full += 1;
        }
        else break;
    }
	while(true)
	{
		Talks session;
		if ( combineTalks(talks, session, m_afternoonSessionMin, m_afternoonSessionMax) )
		{
			sessions.push_back(session);
		}
		else break;
	}
	return inc_full;
}

void SequenceVisitor::visit(TrackExtractor * const e)
{
    UNUSED(e);
    #ifdef DEBUG
        cout << "SequenceVisitor visit TrackExtractor" << endl;        
    #endif
}

void SequenceVisitor::visit(TwoSessionTrackExtractor * const e)
{
    #ifdef DEBUG
        cout << "SequenceVisitor visit TwoSessionTrackExtractor" << endl;
    #endif
    int i = 0;
    SessionsIter morn_iter = e->m_morningSessions.begin();
    SessionsIter after_iter = e->m_afternoonSessions.begin();
    while (morn_iter != e->m_morningSessions.end())
    {
        int hour = 9;
        int minutes = 0; 
        cout << endl;
        i++;
        Talks morn_session = *morn_iter++;
        cout << "Track " << i << endl;
        for (TalksIter iter = morn_session.begin(); iter != morn_session.end(); iter++)
        {
            if ( iter == morn_session.begin() )
                cout << "09:00AM" << " " << iter->m_title << endl;
            else
            {
                cout << e->formatTime(hour, minutes) << "AM " << iter->m_title << endl;
            }
            hour += (minutes + iter->m_mins)/60;
            minutes = (minutes + iter->m_mins)%60;
        }
        if(after_iter == e->m_afternoonSessions.end()) continue;
        Talks after_session = *after_iter++;
        cout << "12:00PM Lunch"<< endl;
        hour = 1;
        minutes = 0;
        for (TalksIter iter = after_session.begin(); iter != after_session.end(); iter++)
        {
            if ( iter == after_session.begin() )
                cout << "01:00PM" << " " << iter->m_title << endl;
            else
            {
                cout << e->formatTime(hour, minutes) << "PM " << iter->m_title << endl;
            }
            hour += (minutes + iter->m_mins)/60;
            minutes = (minutes + iter->m_mins)%60;
        }
        if ( hour >= 4 )
            cout << e->formatTime(hour, minutes) << "PM " << "Networking Event" << endl;
        else
            cout << "04:00PM" << " " << "Networking Event" << endl;
    }
}
