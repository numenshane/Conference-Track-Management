#include "Talk.h"
#include <vector>

#ifndef __EXTRACTOR_H__
#define __EXTRACTOR_H__
class Visitor;
bool TalkCompare (Talk i,Talk j);

typedef vector<Talk> Talks;
typedef Talks::iterator TalksIter;
typedef vector<Talks> Sessions;
typedef Sessions::iterator SessionsIter;

class TrackExtractor {
public:
    TrackExtractor(const Talks& talks);
    virtual bool Extract() ;
    virtual void accpet(Visitor &v) ;
    virtual ~TrackExtractor() {}
    string formatTime(int hour, int minutes);
protected:
    bool combineTalks(Talks& talks, Talks& chosen_talks, int min_minutes, int max_miutes);
    void calculateTrackRange(int minTrackMinutes, int maxTrackMinutes);
protected:
    // originalTalks
    Talks m_talks;

    int m_totalMinutes;
    // track scope
    int m_trackRangeMax;
    int m_trackRangeMin;
};

//thread-safe tool class
class TwoSessionTrackExtractor: public TrackExtractor{
public:
    bool Extract();
    void accept(Visitor &v);
    TwoSessionTrackExtractor(const Talks&  talks, int mornMin, int mornMax, int afterMin, int afterMax);
private:
    bool GetLimitedMorningSession(Talks& talks, vector<Talks>& sessions);
    int GetMaxAfternoonSession(Talks& talks, vector<Talks>& sessions);
    
private:
    // morning session minutes range
    int m_morningSessionMin;
    int m_morningSessionMax;

    int m_trackRangeMorningMax;
    int m_trackRangeMorningMin;

    // afternoon session minutes range
    int m_afternoonSessionMin;
    int m_afternoonSessionMax;
    
public:
    // combine talks to create MorningSession array
    Sessions m_morningSessions;
    // combine talks to create AfternoonSession array
    Sessions m_afternoonSessions;
};

class Visitor{
public:
    virtual void visit(TrackExtractor* e) = 0;
    virtual void visit(TwoSessionTrackExtractor* e) = 0;
};

class SequenceVisitor: public Visitor
{
public:
    void visit(TrackExtractor* e);
    void visit(TwoSessionTrackExtractor* e);
};

#endif
