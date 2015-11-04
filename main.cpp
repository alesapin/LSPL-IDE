#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <Text/maintextviewer.h>
#include <cstdlib>
#include <QtGui>
int main(int argc, char *argv[])
{
    try{
    setenv("RML","/usr/share/aot/",1);
    //QApplication::setStyle("Windows");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    }catch(...){
        std::cerr<<"ALL CATCHED!\n";
    }
}
