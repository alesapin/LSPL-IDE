#include "patternhighlighter.h"

PatternHighlighter::PatternHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    speechPartFormat.setForeground(Qt::darkBlue);
    speechPartFormat.setFontWeight(QFont::Bold);
    separatorsFormat.setForeground(Qt::darkGreen);
    separatorsFormat.setFontItalic(true);
    separatorsFormat.setFontWeight(QFont::Bold);
    paramsFormat.setForeground(Qt::magenta);
    paramsFormat.setFontItalic(true);
    QStringList speechPartPatterns;
    //W N A V Pa Ap Pn Av W1 N1 A1 V1 Pa1 Ap1 Pn1 Av1 W2 N2 A2 V2 Pa2 Ap2 Pn2 Av2 W3 N3 A3 V3 Pa3 Ap3 Pn3 Av3
    speechPartPatterns << "\\bN[1-9]*\\b" << "\\bW[1-9]*\\b" << "\\bA[1-9]*\\b" <<
                          "\\bV[1-9]*\\b" << "\\bPa[1-9]*\\b" << "\\bAp[1-9]*\\b" << "\\bPn[1-9]*\\b" << "\\bAv[1-9]*\\b";
    for(QString& str:speechPartPatterns){
        rule.pattern = QRegExp(str);
        rule.format = speechPartFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("(\"[^\"]*\")|(<[^<]*>)");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("=text>");
    rule.format = separatorsFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegExp("=pattern>");
    rule.format = separatorsFormat;
    highlightingRules.append(rule);
}

void PatternHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule: highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
