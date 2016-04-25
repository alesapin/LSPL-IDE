#ifndef MORPHOLOGYVIEWER_H
#define MORPHOLOGYVIEWER_H
#include <QTextEdit>
#include <QTextCursor>
#include <QHelpEvent>
#include <QMouseEvent>
#include <QTextCharFormat>
#include <QDebug>
#include <QToolTip>
#include "Engine/morphan.h"
class MorphologyViewer : public QTextEdit
{
    Q_OBJECT
private:
    QTextCharFormat* selectedFormat;
    MorphAn morph;
    QString getToolTip(const QString& word) const;
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
public:
    MorphologyViewer(QWidget* parent = 0);


};

#endif // MORPHOLOGYVIEWER_H
