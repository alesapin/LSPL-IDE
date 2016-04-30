#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "centralwidget.h"
#include "Interface/bottombar.h"
class CentralWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    static const QString READY;
    static const QString ENGINE_WORKING;
    static const QString HIGHLIGHT_PROCESSING;
    static const QString PATTERN_COMPILING;
    BottomBar* bar;
    CentralWidget* center;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void slotStatusReady();
    void slotStatusEngine();
    void slotStatusHighlighting();
    void slotStatusCompiling();
};

#endif // MAINWINDOW_H
