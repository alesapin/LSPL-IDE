#ifndef PATTERNCOMPILER_H
#define PATTERNCOMPILER_H
#include <QtGlobal>
#include <lspl/Namespace.h>
#include <lspl/transforms/TextTransformBuilder.h>
#include <lspl/transforms/PatternTransformBuilder.h>
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
#include <string>
#include <QStringList>
#include <iostream>
#include <QMessageBox>
#include <fstream>
#include <QTime>
enum PatternType{
    RIGHT_PART_TEXT,RIGHT_PART_PATTERN,NO_RIGHT_PART
};

class PatternCompiler
{
public:
    struct MatchRepr{
        QString text;
        QString params;
        QString transform;
        //QStringList patternNames;
        uint start;
        uint end;
        uint variants;
    };
    PatternCompiler();
    QSharedPointer<QMap<QString,QVector<PatternCompiler::MatchRepr>>> analyzeText(const QStringList &patternNames,const QString& text) ;
    QString compilePattern(const QString& pattern);
    QString compilePatternNoException(const QString& pattern);
    void clear();
private:
    static const QString RIGHT_WITH_PATTERN;
    static const QString RIGHT_WITH_TEXT;
    QString convertToSystem(const std::string& str);
    lspl::NamespaceRef ns;
    lspl::text::readers::PlainTextReader reader;
    lspl::patterns::PatternBuilderRef textTransfromBuilder;
    lspl::patterns::PatternBuilderRef patternTransformBuilder;
    lspl::patterns::PatternBuilderRef noRightPartBuilder;
    MatchRepr convertMatch(lspl::text::MatchRef ref,PatternType t);
    QTextCodec *codec;
};
typedef QMap<QString,QVector<PatternCompiler::MatchRepr>> PatternViewMap;

#endif // PATTERNCOMPILER_H
