#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    TextBasicWidget* textPart = new TextBasicWidget(this);
    setMenuBar(new TopMenu(this,textPart));
    setCentralWidget(new PatternsBasicWidget());
}

MainWindow::~MainWindow()
{

}

