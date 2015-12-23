#include "patterneditor.h"
#include <QDebug>

PatternEditor::PatternEditor(QWidget *parent):QTextEdit(parent)
{
    setUpdatesEnabled(true);
    this->setAlignment(Qt::AlignLeft);
}


QString PatternEditor::getText() const
{
    return toPlainText().simplified();
}

QStringList PatternEditor::getPatternsForCompile() const
{
    QStringList text = toPlainText().split("\n");
    QStringList result;
    for(QString pattern: text){
        if(!pattern.isEmpty()){
            result.append(pattern);
        }
    }
    return result;
}


void PatternEditor::clearAll()
{
    clear();
}



