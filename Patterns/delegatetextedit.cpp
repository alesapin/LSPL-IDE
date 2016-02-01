#include "delegatetextedit.h"


DelegateTextEdit::DelegateTextEdit(QWidget *parent): QTextEdit(parent){
    new PatternHighlighter(this->document());
}

void DelegateTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter  ||  e->key() == Qt::Key_Return ){
        emit editingFinished();
    } else {
        QTextEdit::keyPressEvent( e );
    }
}
