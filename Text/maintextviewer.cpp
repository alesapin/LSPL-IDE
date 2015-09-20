#include "maintextviewer.h"
#include <QDebug>
MainTextViewer::MainTextViewer(QWidget *parent) : QTextEdit(parent),modified(false)
{
    //installEventFilter(this);
    fmt = new QTextCharFormat();
    cursor = new QTextCursor(this->document());

    fmt->setBackground(Qt::yellow);
    converter = QTextCodec::codecForLocale();
    setMinimumHeight(300);
//    setMinimumWidth(300);
    //setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(this,SIGNAL(textChanged()),this,SLOT(modify()));
}

bool MainTextViewer::isModified()
{
    qDebug() << modified;
    return modified>1;
}

void MainTextViewer::highlightFragment(int begin, int end)
{
    cursor->setPosition(begin,QTextCursor::MoveAnchor);
    cursor->setPosition(end,QTextCursor::KeepAnchor);
    cursor->setCharFormat(*fmt);
}

void MainTextViewer::highlightFragments(const QVector<QPair<int, int> > &frags)
{
    for(QPair<int,int> par:frags){
        highlightFragment(par.first,par.second);
    }
}

void MainTextViewer::selectText(int begin, int end)
{
    QTextCursor crs = this->textCursor();
    crs.setPosition(begin);
    crs.setPosition(end,QTextCursor::KeepAnchor);
    this->setTextCursor(crs);

}

bool MainTextViewer::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip)
        {
            QHelpEvent* helpEvent = static_cast<QHelpEvent*>(e);
            QTextCursor cursor = cursorForPosition(helpEvent->pos());
            cursor.select(QTextCursor::WordUnderCursor);
            if (!cursor.selectedText().isEmpty()){
                QToolTip::showText(helpEvent->globalPos(), cursor.selectedText());
            }else{
                QToolTip::hideText();
            }
            return true;
        }
        return QTextEdit::event(e);
}



void MainTextViewer::modify()
{
    modified ++;
}



//void MainTextViewer::insertFromMimeData(const QMimeData *source)
//{
//    //qDebug() << source->data();
//}

