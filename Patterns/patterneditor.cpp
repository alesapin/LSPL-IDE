#include "patterneditor.h"
#include <QDebug>

PatternEditor::PatternEditor(QWidget *parent):QTextEdit(parent),cachePosition(0)
{
    setUpdatesEnabled(true);
    this->setAlignment(Qt::AlignLeft);
    highlighter = new PatternHighlighter(this->document());
}


QString PatternEditor::getText() const
{
    return toPlainText().simplified();
}


void PatternEditor::clean()
{
    QString current = toPlainText();
    if(!current.isEmpty()){
        textCache.append(current);
        cachePosition = textCache.length() - 1;
    }
    setText("");
}

void PatternEditor::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Up){
        if(textCache.length() > cachePosition && cachePosition >= 0 ){
            setText(textCache[cachePosition--]);
        }
    } else if (e->key() == Qt::Key_Down){
        if(textCache.length() - 1 == cachePosition || textCache.length() == 1){
                    setText("");
                    cachePosition = textCache.length() - 1;
        } else if(cachePosition == -1 && textCache.length() > 1){
            ++cachePosition;
            setText(textCache[++cachePosition]);
        } else if(textCache.length()-1 > cachePosition ){
            setText(textCache[++cachePosition]);
        }
    }
    QTextEdit::keyPressEvent(e);
}



void PatternEditor::clearAll()
{
    clear();
}



