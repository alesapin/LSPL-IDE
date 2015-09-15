#include "patterneditortab.h"





PatternEditorTab::PatternEditorTab(QWidget *parent):QTabWidget(parent)
{
    editor = new PatterEditor();
    addTab(editor,"Pattern Editor");
    setCurrentWidget(editor);
}

QString PatternEditorTab::getText()
{
    return editor->toPlainText();
}
