#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    bar = new BottomBar(this);
    CentralWidget *w = new CentralWidget();
    setMenuBar(new TopMenu(this,w));
    setStatusBar(bar);
    //setCentralWidget(new PatternsBasicWidget());
    setCentralWidget(w);
    connect(w,SIGNAL(statusReady()),this,SLOT(slotStatusReady()));
    connect(w,SIGNAL(statusEngine()),this,SLOT(slotStatusEngine()));
    connect(w,SIGNAL(statusHighlighting()),this,SLOT(slotStatusHighlighting()));
    connect(w,SIGNAL(statusCompiling()),this,SLOT(slotStatusCompiling()));

}

MainWindow::~MainWindow()
{

}

void MainWindow::slotStatusReady()
{
    bar->showMessage(READY);
}

void MainWindow::slotStatusEngine()
{
    bar->showMessage(ENGINE_WORKING);
}

void MainWindow::slotStatusHighlighting()
{
    bar->showMessage(HIGHLIGHT_PROCESSING);
}

void MainWindow::slotStatusCompiling()
{
    bar->showMessage(PATTERN_COMPILING);
}

const QString MainWindow::READY = "Готов";
const QString MainWindow::ENGINE_WORKING = "Шаблоны накладываются ...";
const QString MainWindow::HIGHLIGHT_PROCESSING = "Текст размечается ...";
const QString MainWindow::PATTERN_COMPILING = "Шаблоны компилируются ...";
