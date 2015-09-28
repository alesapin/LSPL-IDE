#include "patterneditortab.h"





PatternEditorTab::PatternEditorTab(QWidget *parent):QTabWidget(parent)
{
    editor = new PatternEditor();
    addTab(editor,"Pattern Editor");
    setCurrentWidget(editor);
}

QString PatternEditorTab::getText()
{
    return editor->toPlainText();
}
