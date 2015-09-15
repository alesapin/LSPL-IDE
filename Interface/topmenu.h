#ifndef TOPMENU_H
#define TOPMENU_H
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QFileDialog>

#include "mainwindow.h"
#include "Text/textbasicwidget.h"
class MainWindow;
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
    QString filename;
public:
    TopMenu(MainWindow* parent = 0,TextBasicWidget* text=0);
private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void saveAllFiles();
};

#endif // TOPMENU_H
