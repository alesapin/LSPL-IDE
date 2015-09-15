#ifndef PATTEREDITOR_H
#define PATTEREDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
class PatterEditor:public QPlainTextEdit
{
public:
    PatterEditor(QWidget* parent=0);
};

#endif // PATTEREDITOR_H
