#ifndef PATTERNHIGHLIGHTER_H
#define PATTERNHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QVector>
#include <QRegExp>
class PatternHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    PatternHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat speechPartFormat;
    QTextCharFormat paramsFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat separatorsFormat;
//    QTextCharFormat singleLineCommentFormat;
//    QTextCharFormat multiLineCommentFormat;
//    QTextCharFormat functionFormat;
};

#endif // PATTERNHIGHLIGHTER_H
