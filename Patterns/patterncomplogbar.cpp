#include "patterncomplogbar.h"

PatternCompLogBar::PatternCompLogBar(QWidget *parent):QTextEdit(parent)
{
    setReadOnly(true);
}

void PatternCompLogBar::clear()
{
    setText("");
}

