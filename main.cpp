#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <Text/maintextviewer.h>
#include <cstdlib>
#include <QtGui>
#include <QDir>
#include <cstdlib>
#include "Utils/intervalrbtree.h"
int main(int argc, char *argv[])
{
//    IntervalRBTree<int> tr;
//    for(int i = 0;i<10;++i){
//        tr.addInterval(i*10,i*100,i);
//    }
//    qDebug() << tr.size();
////    for(int i = 0;i<10;++i){
////        qDebug()<<tr.getEqualInterval(i*10,i*100);
////    }
//    std::vector<int> r = tr.getAllIntersections(900,1000);
//    for(int i =0;i<r.size();++i){
//        qDebug() << r[i];
//    }
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
