#include "patterncompiler.h"
#include <QDebug>
PatternCompiler::MatchRepr PatternCompiler::convertMatch(lspl::text::MatchRef ref)
{
    QByteArray arr = QByteArray::fromStdString(ref->getRangeString());
    QString text = codec->toUnicode(arr);
    return {
         text,
        QString(""),
        QString(""),
        ref->getRangeStart(),
        ref->getRangeEnd()
    };
}

PatternCompiler::PatternCompiler()
{
    ns = new lspl::Namespace();
    builder = new lspl::patterns::PatternBuilder(ns);
    codec = QTextCodec::codecForName("Windows-1251");
}

PatternViewMap PatternCompiler::analyzeText(const QVector<QString> &patternNames, const QString &text)
{
    lspl::text::TextRef txt = reader.readFromString(text.toStdString(),"UTF-8");
    PatternViewMap result;
    for(QString pattern: patternNames){
        lspl::patterns::PatternRef pat = ns->getPatternByName(pattern.toStdString());
        lspl::text::MatchList matches = txt->getMatches(pat);
        QVector<MatchRepr> resultPattern;
        for(uint i = 0; i < matches.size(); ++i){
            lspl::text::MatchRef ref = matches[i];
            MatchRepr rep = convertMatch(matches[i]);
            rep.context = text.mid(std::max(rep.start-5,(uint)0),std::min((int)rep.end+5,text.length())).replace("\n"," ");

            resultPattern.append(rep);
        }
        result[pattern]=resultPattern;
    }
    return result;
}

QString PatternCompiler::compilePattern(const QString& pattern)
{
    try{
        builder->build(pattern.toStdString());
        return "";
    }catch(lspl::patterns::PatternBuildingException e){
        return e.what();
    }
}

