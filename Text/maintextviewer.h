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
#include "Engine/patterncompiler.h"
#include "Utils/intervalrbtree.h"
#include <QSharedPointer>
#include "Utils/util.hpp"
class MainTextViewer : public QPlainTextEdit
{
    Q_OBJECT
private:
    QTextCodec* converter;
    int modified;
    QTextCharFormat* fmtSelect;
    QTextCharFormat* fmtDeSelect;
    QTextCursor* cursor;
    QSharedPointer<utility::IntervalViewMap> intervalMatches;
    QSet<QString> offPatterns;
    QString getToolTip(int start,int end) const;
public:
    explicit MainTextViewer(QWidget *parent = 0);
   // void insertFromMimeData(const QMimeData *source);
    bool isModified();
    void highlightFragment(int begin,int end);
    void deHighlightFragment(int begin,int end);
    void dehighlightAll();
    void setMatches(QSharedPointer<utility::IntervalViewMap> m);
    void highlightPatterns(const QStringList& patternNames);
    void highlightAll();
    //void dehighlightPatterns(const QStringList& patternNames);
    void dehighlightPattern(const QString& pattern);
    void selectText(int begin,int end);
    bool event(QEvent *e);
signals:

public slots:
    void modify();
};

#endif // MAINTEXTVIEWER_H
