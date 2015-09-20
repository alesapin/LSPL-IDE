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
class MainTextViewer : public QTextEdit
{
    Q_OBJECT
private:
    QTextCodec* converter;
    int modified;
    QTextCharFormat* fmt;
    QTextCursor* cursor;
public:
    explicit MainTextViewer(QWidget *parent = 0);
   // void insertFromMimeData(const QMimeData *source);
    bool isModified();
    void highlightFragment(int begin,int end);
    void highlightFragments(const QVector<QPair<int,int>>& frags);
    void selectText(int begin,int end);
    bool event(QEvent *e);
signals:

public slots:
    void modify();
};

#endif // MAINTEXTVIEWER_H
