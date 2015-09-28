#ifndef PATTERNEDITORTAB_H
#define PATTERNEDITORTAB_H
#include <QTabWidget>
#include "patterneditor.h"

class PatternEditorTab : public QTabWidget
{
private:
    PatternEditor* editor;
public:
    PatternEditorTab(QWidget* parent=0);
    QString getText();
};

#endif // PATTERNEDITORTAB_H
