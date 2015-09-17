#ifndef TOPMENU_H
#define TOPMENU_H
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QFileDialog>

#include "mainwindow.h"
#include "../centralwidget.h"
class CentralWidget;
class TextBasicWidget;
class TopMenu : public QMenuBar
{
    Q_OBJECT
private:
    QMenu* fileMenu;
    QMenu* patternsMenu;
    QMenu* windowMenu;
    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* saveAllAct;
    QAction* exitAct;
    void setFileMenu();
    void setFileActions();
    TextBasicWidget* text;
    CentralWidget* cent;
    QString filename;
public:
    TopMenu(QWidget* parent = 0,CentralWidget* c=0);
private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void saveAllFiles();
};

#endif // TOPMENU_H
