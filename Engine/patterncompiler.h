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
class PatternCompiler
{
public:
    struct MatchRepr{
        QString text;
        QString context;
        QString params;
        uint start;
        uint end;
    };
    PatternCompiler();
    QVector<MatchRepr> analyzeText(const QVector<QString> &patternNames,const QString& text);
    QString compilePattern(const QString& pattern);
private:
    lspl::NamespaceRef ns;
    lspl::patterns::PatternBuilderRef builder;
    lspl::text::readers::PlainTextReader reader;
    MatchRepr convertMatch(lspl::text::MatchRef ref);
    QTextCodec *codec;
};

#endif // PATTERNCOMPILER_H
