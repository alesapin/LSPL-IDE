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

QDomDocument toXml(const PatternViewMap &matches)
{

    QDomDocument doc;
    QDomElement header1 = doc.createElement("texts");
    QDomElement text = doc.createElement("text");
    for(QString pattern:matches.keys()){
        QDomElement ptrn = doc.createElement("goal");
        ptrn.setAttribute("name",pattern);
        for(PatternCompiler::MatchRepr m: matches[pattern]){
           QDomElement mtch = doc.createElement("match");
           mtch.setAttribute("startPos",m.start);
           mtch.setAttribute("endPos",m.end);
           QDomElement frag = doc.createElement("fragment");
           QDomText text = doc.createTextNode(m.text);
           frag.appendChild(text);
           mtch.appendChild(frag);
           if(!m.transform.isEmpty()){
                QDomElement res = doc.createElement("result");
                QDomText trns = doc.createTextNode(m.transform);
                res.appendChild(trns);
                mtch.appendChild(res);
           }
           ptrn.appendChild(mtch);
        }
        text.appendChild(ptrn);
    }
    header1.appendChild(text);
    doc.appendChild(header1);
    return doc;
}

QDomDocument toXml(QSharedPointer<IntervalViewMap> matches)
{
    QDomDocument doc;
    QDomElement header1 = doc.createElement("texts");
    QDomElement text = doc.createElement("text");
    for(IntervalViewMap::iterator it = matches->begin(); it != matches->end(); ++it){
        IntervalMatch current = it->value;
        QDomElement interval = doc.createElement("interval");
        interval.setAttribute("start",it->low);
        interval.setAttribute("end",it->high);
        interval.setAttribute("text",current.text);

        for(int i = 0;i<current.patterns.size();++i){
            QDomElement pattern = doc.createElement("pattern");
            QDomElement name = doc.createElement("name");
            QDomText nameText = doc.createTextNode(current.patterns[i]);
            name.appendChild(nameText);
            pattern.appendChild(name);
            if(!current.transforms[i].isEmpty()) {
                QDomElement transform = doc.createElement("transform");
                QDomText transText = doc.createTextNode(current.transforms[i]);
                transform.appendChild(transText);
                pattern.appendChild(transform);
            }
            if(!current.params[i].isEmpty()){
                QDomElement params = doc.createElement("params");
                QDomText paramsText = doc.createTextNode(current.params[i]);
                params.appendChild(paramsText);
                pattern.appendChild(params);
            }
            interval.appendChild(pattern);
            text.appendChild(interval);
        }
    }
    header1.appendChild(text);
    doc.appendChild(header1);
    return doc;
}

const char *codecNameForText(QFile &in)
{
    csd_t csd = csd_open();
    if (csd == (csd_t)-1) {
          return "UTF-8";
    }
   int size;
   char buf[4096] = {0};
   while ((size = in.read(buf,4096)) != 0) {
          int result = csd_consider(csd, buf, size);
          if (result < 0) {
              return "UTF-8";
          } else if (result > 0) {
              break;
          }
      }
      in.seek(0);
      const char *result = csd_close(csd);
      if (result == NULL) {
        return "UTF-8";
      } else {
          return result;
      }
}

}



