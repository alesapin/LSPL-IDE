#ifndef PATTERNEDITORTAB_H
#define PATTERNEDITORTAB_H
#include <QTabWidget>
#include "pattereditor.h"

class PatternEditorTab : public QTabWidget
{
private:
    PatterEditor* editor;
public:
    PatternEditorTab(QWidget* parent=0);
    QString getText();
};

#endif // PATTERNEDITORTAB_H
