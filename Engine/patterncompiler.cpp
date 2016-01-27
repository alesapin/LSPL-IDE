#include "patterncompiler.h"
#include <QDebug>
const QString PatternCompiler::RIGHT_WITH_PATTERN = "=pattern>";
const QString PatternCompiler::RIGHT_WITH_TEXT =  "=text>";

PatternCompiler::MatchRepr PatternCompiler::convertMatch(lspl::text::MatchRef ref,PatternType patternType)
{
    QString text = convertToSystem(ref->getRangeString());
    QString transform;
    QString params;
    int vars = ref->getVariantCount();
    lspl::text::Match::AttributesMap attrMap = ref->getAttributes();
    for(std::map<lspl::text::attributes::AttributeKey,lspl::text::attributes::AttributeValue>::iterator itr = attrMap.begin();
        itr != attrMap.end();++itr)
    {
        QString title = convertToSystem(itr->first.getTitle());
        QString value = convertToSystem(itr->second.getTitle());
        QString otherValue;
        if(value == "неопределено"){
            otherValue = convertToSystem(itr->second.getString());
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
        transform = convertToSystem(p->getName().c_str()) + " = " + convertToSystem(p->getSource().c_str());
        break;
    }
    case RIGHT_PART_TEXT:
    {
        transform = convertToSystem(ref->getVariant(0)->getTransformResult<std::string>().c_str());
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
        ref->getRangeEnd(),
        vars
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

QSharedPointer<PatternViewMap> PatternCompiler::analyzeText(const QStringList &patternNames, const QString &text)
{

    QByteArray arr = codec->fromUnicode(text);
    std::string res(arr.data());
    lspl::text::TextRef txt = reader.readFromString(res);
    PatternViewMap* result = new PatternViewMap;
    for(QString pattern: patternNames){
        PatternType patternType;
        lspl::patterns::PatternRef pat = ns->getPatternByName(pattern.toStdString());
        QString patSource(pat->getSource().c_str());
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
        (*result)[pattern]=resultPattern;
    }
    return QSharedPointer<PatternViewMap>(result);
}

QString PatternCompiler::compilePattern(const QString& pattern)
{
    //qDebug() << pattern;
    std::cerr<<pattern.toStdString()<<"\n";
    try{
        if(pattern.contains(RIGHT_WITH_PATTERN)){
            patternTransformBuilder->build(pattern.toStdString());
        }else if (pattern.contains(RIGHT_WITH_TEXT)){
            textTransfromBuilder->build(pattern.toStdString());
        }else{
            noRightPartBuilder->build(pattern.toStdString());
        }
        return "";
    }catch(const lspl::patterns::PatternBuildingException& e){
        std::cerr << "Catched!\n";
        return "Something went wrong";
    }catch(const std::exception & e){
        std::cerr << "Catched here!\n";
        return "Something went wrong...";
    }
}

QString PatternCompiler::compilePatternNoException(const QString &pattern)
{
    lspl::patterns::PatternBuilder::BuildInfo bi;
    if(pattern.contains(RIGHT_WITH_PATTERN)){
        bi=patternTransformBuilder->buildNoException(pattern.toStdString());
    }else if (pattern.contains(RIGHT_WITH_TEXT)){
        bi=textTransfromBuilder->buildNoException(pattern.toStdString());
    }else{
        bi=noRightPartBuilder->buildNoException(pattern.toStdString());
    }
    if(bi.parseLength == -1){
        return QString::fromStdString(bi.errorMsg);
    } else {
        return "";
    }
}

void PatternCompiler::clear()
{
    ns = new lspl::Namespace();
    noRightPartBuilder = new lspl::patterns::PatternBuilder(ns);
    textTransfromBuilder = new lspl::patterns::PatternBuilder(ns, new lspl::transforms::TextTransformBuilder(ns));
    patternTransformBuilder = new lspl::patterns::PatternBuilder(ns, new lspl::transforms::PatternTransformBuilder(ns));
}

QString PatternCompiler::convertToSystem(const std::string &str)
{
    QByteArray arr = QByteArray::fromStdString(str);
    return codec->toUnicode(arr).toLower();

}

