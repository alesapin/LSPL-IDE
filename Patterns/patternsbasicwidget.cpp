#include "patternsbasicwidget.h"

PatternsBasicWidget::PatternsBasicWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout * lay = new QGridLayout(this);
    editorTab = new PatternEditorTab(this);
    compileButton = new QPushButton("Compile",this);
//    lay->setRowMinimumHeight(0,300);
//    lay->setColumnMinimumWidth(0,300);
    lay->addWidget(compileButton,0,0);
    lay->addWidget((QWidget*)editorTab,1,0);
}

