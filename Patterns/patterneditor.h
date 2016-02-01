#ifndef PATTEREDITOR_H
#define PATTEREDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QVector>
#include <QPainter>
#include <QTextBlock>
#include <QFontMetrics>
#include <QMenu>
#include <QAction>
#include <QSizePolicy>
#include "patternhighlighter.h"
class PatternEditor:public QTextEdit
{
    Q_OBJECT
public:
    PatternEditor(QWidget* parent=0);
    QString getText() const;
    void clean();
protected:
    virtual void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
public slots:
    void clearAll();
private:
    PatternHighlighter* highlighter;
    QVector<QString> textCache;
    int cachePosition;
};

#endif // PATTEREDITOR_H
