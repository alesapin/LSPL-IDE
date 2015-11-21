#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <Text/maintextviewer.h>
#include <cstdlib>
#include <QtGui>
#include <QDir>
int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QString path = "RML="+QDir::currentPath()+"/aot";
    putenv(path.toStdString().c_str());
#else
    setenv("RML","/usr/share/aot/",1);
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
