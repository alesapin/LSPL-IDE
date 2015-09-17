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

QVector<PatternCompiler::MatchRepr> PatternCompiler::analyzeText(const QVector<QString> &patternNames, const QString &text)
{
    lspl::text::TextRef txt = reader.readFromString(text.toStdString(),"UTF-8");
    QVector<MatchRepr> result;
    for(QString pattern: patternNames){
        lspl::patterns::PatternRef pat = ns->getPatternByName(pattern.toStdString());
        lspl::text::MatchList matches = txt->getMatches(pat);
        for(uint i = 0; i < matches.size(); ++i){
            MatchRepr rep = convertMatch(matches[i]);
            rep.context = text.mid(std::max(rep.start-10,(uint)0),std::min(rep.end-rep.start+10,text.length()-rep.start));
            result.append(rep);
        }
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

