#include "mainwindow.h"
#include <QApplication>
#include <Text/maintextviewer.h>
#include <cstdlib>
int main(int argc, char *argv[])
{
    setenv("RML","/usr/share/aot/",1);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
