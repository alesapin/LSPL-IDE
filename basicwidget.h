#ifndef BASICWIDGET_H
#define BASICWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QLabel>
#include <QVariant>
#include <QLayout>
#include <QStyle>
#include <QDebug>
#include <QResizeEvent>
class BasicWidget: public QDockWidget
{
public:
    BasicWidget(QWidget* parent = 0,const QString& name="");
protected:
    void resizeEvent(QResizeEvent* e);
private:
    QWidget *wrapper;
};

#endif // BASICWIDGET_H
