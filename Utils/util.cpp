#include "util.hpp"
namespace utility {
QSharedPointer<IntervalViewMap> convertMatchesToIntervals(const PatternViewMap& m){
    IntervalViewMap *intervalMatches = new IntervalViewMap();
    for(auto it = m.begin();it!=m.end();++it){
        QString patternName = it.key();
        QVector<PatternCompiler::MatchRepr> mts = it.value();
        for(PatternCompiler::MatchRepr match:mts){
            if(intervalMatches->containsEqualInterval(match.start,match.end)){
                IntervalMatch &pm = intervalMatches->getEqualInterval(match.start,match.end);
                pm.patterns.append(patternName);
                pm.params.append(match.params);
                pm.transforms.append(match.transform);
            } else{
                IntervalMatch current;
                current.text = match.text;
                current.patterns.append(patternName);
                current.params.append(match.params);
                current.transforms.append(match.transform);
                intervalMatches->addInterval(match.start,match.end,current);
            }
        }
    }

    return QSharedPointer<IntervalViewMap>(intervalMatches);
}
}



