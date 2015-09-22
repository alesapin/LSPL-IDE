#ifndef MAINTEXTVIEWER_H
#define MAINTEXTVIEWER_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QMimeData>
#include <QTextCodec>
#include <QPair>
#include <QTextFormat>
#include <QTextCursor>
#include <QVector>
#include <QHelpEvent>
#include <QToolTip>
#include <Engine/patterncompiler.h>
class MainTextViewer : public QPlainTextEdit
{
    Q_OBJECT
private:
    QTextCodec* converter;
    int modified;
    QTextCharFormat* fmtSelect;
    QTextCharFormat* fmtDeSelect;
    QTextCursor* cursor;
    PatternViewMap matches;
    const QVector<QPair<int,int>> getIntervalsForPattern(const QString& patternName);
public:
    explicit MainTextViewer(QWidget *parent = 0);
   // void insertFromMimeData(const QMimeData *source);
    bool isModified();
    void highlightFragment(int begin,int end);
    void highlightFragments(const QVector<QPair<int,int>>& frags);
    void deHighlightFragment(int begin,int end);
    void deHighlightFragments(const QVector<QPair<int,int>>& frags);
    void setMatches(const PatternViewMap& m);
    void highlightPatterns(const QStringList& patternNames);
    void dehighlightPatterns(const QStringList& patternNames);
    void selectText(int begin,int end);
    bool event(QEvent *e);
signals:

public slots:
    void modify();
};

#endif // MAINTEXTVIEWER_H
