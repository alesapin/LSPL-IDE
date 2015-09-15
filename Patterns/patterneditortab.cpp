#include "patterneditortab.h"





PatternEditorTab::PatternEditorTab(QWidget *parent):QTabWidget(parent)
{
    PatterEditor* editor = new PatterEditor();
    addTab(editor,"Pattern Editor");
    setCurrentWidget(editor);
}
