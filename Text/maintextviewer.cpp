#include "maintextviewer.h"
#include <QDebug>
MainTextViewer::MainTextViewer(QWidget *parent) : QTextEdit(parent),modified(false)
{
    //installEventFilter(this);
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



void MainTextViewer::modify()
{
    modified ++;
}



//void MainTextViewer::insertFromMimeData(const QMimeData *source)
//{
//    //qDebug() << source->data();
//}

