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
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>
class MainTextViewer : public QPlainTextEdit
{
    Q_OBJECT
private:
    QTextCodec* converter;
    int modified;
    QTextCharFormat* fmtDeSelect;
    QTextCharFormat* fmtSelect;
    QTextCursor* cursor;
    QTextCursor* selectionCursor;
    QSharedPointer<utility::IntervalViewMap> intervalMatches;
    QSet<QString> offPatterns;
    QList<QTextEdit::ExtraSelection> tmpSelections;
    QFutureWatcher<void>* watcher;
    QSet<QString> highlightedPatterns;

    void parallelHighlightAll();
    void parallelHighlightPatterns();
    void parallelDehighlightPattern();

    QString getToolTip(int start,int end) const;

public:
    explicit MainTextViewer(QWidget *parent = 0);
   // void insertFromMimeData(const QMimeData *source);
    bool isModified();
    void dehighlightAll();
    void setMatches(QSharedPointer<utility::IntervalViewMap> m);
    void highlightPatterns(const QStringList& patternNames);
    void highlightAll();
    void dehighlightPattern(const QString& pattern);
    void selectText(int begin,int end);
    void stopCalcing();
    bool event(QEvent *e);
signals:
    void highlightIt(int,int,bool);
    void chekingEnabled();
public slots:
    void slotHighlightPatterns();
    void slotDehighlightPattern();
    void slotHighlightAll();
    void slotModify();
    void slotHighlighFragment(int,int,bool);

};

#endif // MAINTEXTVIEWER_H
