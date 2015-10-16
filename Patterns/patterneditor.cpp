#include "patterneditor.h"
#include <QDebug>

PatternEditor::PatternEditor(QWidget *parent):QTextEdit(parent)
{
    setUpdatesEnabled(true);
    setMinimumHeight(300);
    this->setAlignment(Qt::AlignLeft);
}


QString PatternEditor::getText() const
{
    QStringList text = toPlainText().split("\n");
    QString result;
    for(QString s:text){
        if(!s.simplified().isEmpty()){
            result += s + '\n';
        }
    }
    return result;
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



