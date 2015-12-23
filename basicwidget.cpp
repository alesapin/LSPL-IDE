#include "basicwidget.h"

BasicWidget::BasicWidget(QWidget *parent,const QString& name): QDockWidget(parent)
{
    setContentsMargins(3,3,3,3);
    QLabel* title = new QLabel("<strong>"+name+"<strong>");
    title->setTextFormat(Qt::RichText);
    QHBoxLayout* lay = new QHBoxLayout();
    lay->setContentsMargins(3,3,0,0);
    lay->addWidget(title);
    wrapper = new QWidget();
    wrapper->setContentsMargins(0,0,0,0);
    wrapper->setLayout(lay);
    wrapper->adjustSize();
    wrapper->setObjectName("dummy");
    wrapper->setStyleSheet("QWidget#dummy {border:0px;}");
//    QRect currentSize = lab->geometry();
//    lab->setGeometry(10,3,currentSize.width()-3,currentSize.height()-3);
    title->setProperty("class",QVariant("dockHeader"));
    title->setContentsMargins(3,3,3,3);
    //title->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    //wrapper->setFixedWidth(400);
    //wrapper->setFixedWidth(400);
    setTitleBarWidget(wrapper);

}

void BasicWidget::resizeEvent(QResizeEvent *e)
{
    QSize s = e->size();
    wrapper->setFixedWidth(s.width()-3);
    QDockWidget::resizeEvent(e);

}

