#include "xmlprocessor.h"

namespace utility {

const QString XmlProcessor::MAIN_TAG = "lspl";
const QString XmlProcessor::TEXT_TAG = "text";
const QString XmlProcessor::PATTERNS_TAG = "patterns";
const QString XmlProcessor::PATTERN_TAG = "pattern";
const QString XmlProcessor::MATCHES_TAG = "matches";
const QString XmlProcessor::MATCH_TAG = "interval";
const QString XmlProcessor::MATCH_START_ATTR = "start";
const QString XmlProcessor::MATCH_END_ATTR = "end";
const QString XmlProcessor::MATCH_TEXT_ATTR = "text";
const QString XmlProcessor::MATCH_PATTERN_TAG = "pattern";
const QString XmlProcessor::MATCH_PATTERN_NAME_TAG = "name";
const QString XmlProcessor::MATCH_PATTERN_TRANS_TAG = "transform";
const QString XmlProcessor::MATCH_PATTERN_PARAMS_TAG = "params";
const QString XmlProcessor::MATCH_VARIANT_TAG = "variants";

bool XmlProcessor::readMatches(const QDomElement &matchElem)
{
    if(matchElem.nodeName() != MATCHES_TAG) return false;
    matches = QSharedPointer<IntervalViewMap>(new IntervalViewMap());
    QDomNodeList matchNodes = matchElem.childNodes();
    for (int i = 0;i<matchNodes.size();++i) {
        QDomElement match = matchNodes.at(i).toElement();
        if(match.nodeName() == MATCH_TAG){
            QString startS = match.attribute(MATCH_START_ATTR);
            QString endS = match.attribute(MATCH_END_ATTR);
            QString text = match.attribute(MATCH_TEXT_ATTR);
            IntervalMatch currentMatch;
            currentMatch.text = text;
            if(!startS.isEmpty() && !endS.isEmpty()){
                int start = startS.toInt();
                int end = endS.toInt();
                QDomNodeList patternsNodes = match.childNodes();
                for(int i = 0; i<patternsNodes.size();++i){
                    QDomElement pattern = patternsNodes.at(i).toElement();
                    QString patternName;
                    QString patternTransform;
                    QString patternParams;
                    int variants = 1;
                    QDomNodeList patName = pattern.elementsByTagName(MATCH_PATTERN_NAME_TAG);
                    if(patName.size() == 1){
                        patternName = patName.at(0).toElement().text();
                        if(patternName.isEmpty()) return false;
                    }
                    QDomNodeList trans = pattern.elementsByTagName(MATCH_PATTERN_TRANS_TAG);
                    if(trans.size() == 1){
                        patternTransform = trans.at(0).toElement().text();
                    }
                    QDomNodeList params = pattern.elementsByTagName(MATCH_PATTERN_PARAMS_TAG);
                    if(params.size() == 1){
                        patternParams = params.at(0).toElement().text();
                    }
                    QDomNodeList vars = pattern.elementsByTagName(MATCH_PATTERN_PARAMS_TAG);
                    if(vars.size() == 1 && !vars.at(0).toElement().text().isEmpty()){
                        variants = vars.at(0).toElement().text().toInt();
                    }
                    currentMatch.patterns.append(patternName);
                    currentMatch.transforms.append(patternTransform);
                    currentMatch.params.append(patternParams);
                    currentMatch.variants.append(variants);
                }
                matches->addInterval(start,end,currentMatch);
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
    return true;
}

bool XmlProcessor::readText(const QDomElement &textElem)
{
    if(textElem.nodeName() != TEXT_TAG) return false;
    text = textElem.text();
    return true;
}

bool XmlProcessor::readPatterns(const QDomElement &patElem)
{
    if(patElem.nodeName() != PATTERNS_TAG) return false;
    QDomNodeList pats = patElem.childNodes();
    for(int i = 0; i < pats.size(); ++i){
        QDomElement pattern = pats.at(i).toElement();
        if(pattern.nodeName() == PATTERN_TAG) {
            if(!pattern.text().isEmpty()){
                patterns.append(pattern.text());
            }
        }else {
            return false;
        }
    }
    return true;
}

bool XmlProcessor::writeMatches(QDomElement &matchElem)
{
    if(matches){
        for(IntervalViewMap::iterator it = matches->begin(); it != matches->end(); ++it){
            IntervalMatch current = it->value;
            QDomElement interval = maindoc.createElement(MATCH_TAG);
            interval.setAttribute(MATCH_START_ATTR,it->low);
            interval.setAttribute(MATCH_END_ATTR,it->high);
            interval.setAttribute(MATCH_TEXT_ATTR,current.text);
            for(int i = 0;i<current.patterns.size();++i){
                QDomElement pattern = maindoc.createElement(MATCH_PATTERN_TAG);
                QDomElement name = maindoc.createElement(MATCH_PATTERN_NAME_TAG);
                QDomText nameText = maindoc.createTextNode(current.patterns[i]);
                name.appendChild(nameText);
                pattern.appendChild(name);
                if(!current.transforms[i].isEmpty()) {
                    QDomElement transform = maindoc.createElement(MATCH_PATTERN_TRANS_TAG);
                    QDomText transText = maindoc.createTextNode(current.transforms[i]);
                    transform.appendChild(transText);
                    pattern.appendChild(transform);
                }
                if(!current.params[i].isEmpty()){
                    QDomElement params = maindoc.createElement(MATCH_PATTERN_PARAMS_TAG);
                    QDomText paramsText = maindoc.createTextNode(current.params[i]);
                    params.appendChild(paramsText);
                    pattern.appendChild(params);
                }
                interval.appendChild(pattern);
                matchElem.appendChild(interval);
            }
        }
        return true;
    } else {
        return false;
    }
}

bool XmlProcessor::writeText(QDomElement &textElem)
{
    QDomText nodeText = maindoc.createTextNode(text);
    textElem.appendChild(nodeText);
    return true;
}

bool XmlProcessor::writePatterns(QDomElement &patElem)
{
    for(const QString &pattern : patterns){
        QDomElement patNode = maindoc.createElement(PATTERN_TAG);
        QDomText patternText = maindoc.createTextNode(pattern);
        patNode.appendChild(patternText);
        patElem.appendChild(patNode);
    }
    return true;
}

XmlProcessor::XmlProcessor():matches(nullptr)
{

}

bool XmlProcessor::readFile(const QString& name)
{
    QFile mainfile(name);
    if(!mainfile.open(QFile::ReadOnly | QFile::Text)){
       return false;
    }
    maindoc.setContent(&mainfile);
    QDomNodeList txtNode =  maindoc.elementsByTagName(TEXT_TAG);
    if(txtNode.isEmpty() || !readText(txtNode.at(0).toElement())){
        return false;
    }
    QDomNodeList pats = maindoc.elementsByTagName(PATTERNS_TAG);
    if(pats.isEmpty() || !readPatterns(pats.at(0).toElement())){
        return false;
    }
    QDomNodeList mtchs = maindoc.elementsByTagName(MATCHES_TAG);
    if(mtchs.isEmpty() || !readMatches(mtchs.at(0).toElement())){
        return false;
    }
    mainfile.close();
    return true;
}

bool XmlProcessor::writeFile(const QString& name)
{
    QFile mainfile(name);
    QDomDocument doc;
    QDomElement header1 = doc.createElement(MAIN_TAG);
    if(!mainfile.open(QFile::WriteOnly | QFile::Text)){
       return false;
    }
    QDomElement textElem = doc.createElement(TEXT_TAG);
    QDomElement patternsElem = doc.createElement(PATTERNS_TAG);
    QDomElement matchesElem = doc.createElement(MATCHES_TAG);
    if(!writeText(textElem) || !writePatterns(patternsElem) || !writeMatches(matchesElem)){
        return false;
    }
    header1.appendChild(textElem);
    header1.appendChild(patternsElem);
    header1.appendChild(matchesElem);
    doc.appendChild(header1);
    QTextStream out(&mainfile);
    out << doc.toString();
    mainfile.close();
    return true;
}

}
