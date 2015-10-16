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
class PatternEditor:public QTextEdit
{
    Q_OBJECT
public:
    PatternEditor(QWidget* parent=0);
    QString getText() const;
    QStringList getPatternsForCompile() const;
public slots:
    void clearAll();
};

#endif // PATTEREDITOR_H
