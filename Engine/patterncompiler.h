#ifndef PATTERNCOMPILER_H
#define PATTERNCOMPILER_H
#include <lspl/Namespace.h>
#include <lspl/patterns/PatternBuilder.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/text/readers/PlainTextReader.h>
#include <lspl/text/Text.h>
#include <lspl/text/Match.h>
#include <QString>
#include <QVector>
#include <QTextCodec>
#include <algorithm>
#include <QMap>
class PatternCompiler
{
public:
    struct MatchRepr{
        QString text;
        QString context;
        QString params;
        //QVector<QString> variants; Вопросец
        uint start;
        uint end;
    };
    PatternCompiler();
    QMap<QString,QVector<PatternCompiler::MatchRepr>> analyzeText(const QVector<QString> &patternNames,const QString& text);
    QString compilePattern(const QString& pattern);
private:
    lspl::NamespaceRef ns;
    lspl::patterns::PatternBuilderRef builder;
    lspl::text::readers::PlainTextReader reader;
    MatchRepr convertMatch(lspl::text::MatchRef ref);
    QTextCodec *codec;
};
typedef QMap<QString,QVector<PatternCompiler::MatchRepr>> PatternViewMap;

#endif // PATTERNCOMPILER_H
