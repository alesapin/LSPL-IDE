#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <Text/matchtextviewer.h>
#include <QtGui>
#include <QDir>
#include <cstdlib>
#include "Utils/intervalrbtree.h"
#include <QStyleFactory>
#include "proxystyle.h"
int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QString path = "RML="+QDir::currentPath()+"/aot";
    putenv(path.toStdString().c_str());
#else
    setenv("RML","/usr/share/aot/",1);
#endif
    QApplication a(argc, argv);
    QFile file(":/stylesheet/stylesheet.qss");
     if(file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         a.setStyleSheet(file.readAll());
         file.close();
     }
    MainWindow w;
    w.show();
    return a.exec();
}
