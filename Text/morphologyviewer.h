#ifndef MORPHOLOGYVIEWER_H
#define MORPHOLOGYVIEWER_H
#include <QTextEdit>
#include <QTextCursor>
#include <QHelpEvent>
#include <QMouseEvent>
#include <QTextCharFormat>
#include <QDebug>
#include <QEvent>
#include <QToolTip>
#include "Engine/morphan.h"
class MorphologyViewer : public QTextEdit
{
    Q_OBJECT
private:
    QTextCharFormat* selectedFormat;
    MorphAn morph;
    QPoint toolTipPos;
    QWidget *toolTipWidget;
    QString getToolTip(const QString& word) const;
    static const QString FORM_TEMPLATE;
    static const QString TOOLTIP_TEMPLATE;
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    bool event(QEvent *e) override;
public:
    MorphologyViewer(QWidget* parent = 0);


};

#endif // MORPHOLOGYVIEWER_H
