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
    int modified;
    QTextCursor* selectionCursor;
    QSharedPointer<utility::IntervalViewMap> intervalMatches;
    QSet<QString> offPatterns;
    QList<QTextEdit::ExtraSelection> tmpSelections; //При выделении через бекграунд
    QFutureWatcher<void>* watcher;
    QSet<QString> highlightedPatterns;

    void parallelHighlightAll();
    void parallelHighlightPatterns();
    void parallelDehighlightPattern();

    QString getToolTip(int start,int end) const;

public:
    explicit MainTextViewer(QWidget *parent = 0);
    bool isModified();
    void dehighlightAll();
    void setMatches(QSharedPointer<utility::IntervalViewMap> m);
    QSharedPointer<utility::IntervalViewMap> getMatches() const;
    void highlightPatterns(const QStringList& patternNames);
    void highlightAll();
    void dehighlightPattern(const QString& pattern);
    void selectText(int begin,int end);
    void stopCalcing();
    bool event(QEvent *e) Q_DECL_OVERRIDE;
signals:
    void highlightIt(int,int,bool);
    void jobDone();
public slots:
    void slotModify();
    void slotHighlighFragment(int,int,bool);
private slots:
    void slotHighlightPatterns();
    void slotDehighlightPattern();
    void slotHighlightAll();
};

#endif // MAINTEXTVIEWER_H
