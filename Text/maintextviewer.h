#ifndef MAINTEXTVIEWER_H
#define MAINTEXTVIEWER_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QMimeData>
#include <QTextCodec>
class MainTextViewer : public QTextEdit
{
    Q_OBJECT
private:
    QTextCodec* converter;
    int modified;
public:
    explicit MainTextViewer(QWidget *parent = 0);
   // void insertFromMimeData(const QMimeData *source);
    bool isModified();
signals:

public slots:
    void modify();
};

#endif // MAINTEXTVIEWER_H
