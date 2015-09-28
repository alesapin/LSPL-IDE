#include "patterncompiler.h"
#include <QDebug>
const QString PatternCompiler::RIGHT_WITH_PATTERN = "=pattern>";
const QString PatternCompiler::RIGHT_WITH_TEXT =  "=text>";

PatternCompiler::MatchRepr PatternCompiler::convertMatch(lspl::text::MatchRef ref,PatternType patternType)
{
    QString text = convertToUtf(ref->getRangeString());
    QString transform;
    QString params;
    lspl::text::Match::AttributesMap attrMap = ref->getAttributes();
    for(std::map<lspl::text::attributes::AttributeKey,lspl::text::attributes::AttributeValue>::iterator itr = attrMap.begin();
        itr != attrMap.end();++itr)
    {
        QString title = convertToUtf(itr->first.getTitle());
        QString value = convertToUtf(itr->second.getTitle());
        QString otherValue;
        if(value == "неопределено"){
            otherValue = convertToUtf(itr->second.getString());
        }
        if(otherValue != ""){
            value = otherValue;
        }
        if(!title.isEmpty() && ! value.isEmpty()){

            params += title+" : " + value + ", ";
        }
    }
    params.resize(params.length()-2);
    switch(patternType){
    case RIGHT_PART_PATTERN:
    {
        lspl::patterns::PatternRef p = ref->getVariant(0)->getTransformResult<lspl::patterns::PatternRef>();
        transform = convertToUtf(p->getName().c_str()) + " = " + convertToUtf(p->getSource().c_str());
        break;
    }
    case RIGHT_PART_TEXT:
    {
        transform = convertToUtf(ref->getVariant(0)->getTransformResult<std::string>().c_str());
        break;
    }
    default:
        transform = "";
        break;
    }
    return {
         text,
        params,
        transform,
        ref->getRangeStart(),
        ref->getRangeEnd()
    };
}

PatternCompiler::PatternCompiler()
{
    ns = new lspl::Namespace();
    noRightPartBuilder = new lspl::patterns::PatternBuilder(ns);
    textTransfromBuilder = new lspl::patterns::PatternBuilder(ns, new lspl::transforms::TextTransformBuilder(ns));
    patternTransformBuilder = new lspl::patterns::PatternBuilder(ns, new lspl::transforms::PatternTransformBuilder(ns));
    codec = QTextCodec::codecForName("Windows-1251");
}

PatternViewMap PatternCompiler::analyzeText(const QStringList &patternNames, const QString &text)
{
    lspl::text::TextRef txt = reader.readFromString(text.toStdString(),"UTF-8");
    PatternViewMap result;
    for(QString pattern: patternNames){
        PatternType patternType;
        lspl::patterns::PatternRef pat = ns->getPatternByName(pattern.toStdString());
        QString patSource(pat->getSource().c_str());
        qDebug() <<"Pattern Source:"<<patSource;
        if(patSource.contains(RIGHT_WITH_PATTERN)){
            patternType = RIGHT_PART_PATTERN;
        }else if (patSource.contains(RIGHT_WITH_TEXT)){
            patternType = RIGHT_PART_TEXT;
        } else {
            patternType = NO_RIGHT_PART;
        }
        lspl::text::MatchList matches = txt->getMatches(pat);
        QVector<MatchRepr> resultPattern;
        for(uint i = 0; i < matches.size(); ++i){
            MatchRepr rep = convertMatch(matches[i],patternType);
            resultPattern.append(rep);
        }
        result[pattern]=resultPattern;
    }
    return result;
}

QString PatternCompiler::compilePattern(const QString& pattern)
{
    qDebug() << pattern;

    this->textTransfromBuilder;
    try{
        if(pattern.contains(RIGHT_WITH_PATTERN)){
            patternTransformBuilder->build(pattern.toStdString());
        }else if (pattern.contains(RIGHT_WITH_TEXT)){
            textTransfromBuilder->build(pattern.toStdString());
        }else{
            noRightPartBuilder->build(pattern.toStdString());
        }
        return "";
    }catch(lspl::patterns::PatternBuildingException e){
        return e.what();
    }
}

QString PatternCompiler::convertToUtf(const std::string &str)
{
    QByteArray arr = QByteArray::fromStdString(str);
    return codec->toUnicode(arr);
}

