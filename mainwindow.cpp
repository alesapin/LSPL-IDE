#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CentralWidget *w = new CentralWidget();
    setMenuBar(new TopMenu(this,w));
    //setCentralWidget(new PatternsBasicWidget());
    setCentralWidget(w);
}

MainWindow::~MainWindow()
{

}

