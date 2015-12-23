#ifndef TOPMENU_H
#define TOPMENU_H
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QFileDialog>

#include "mainwindow.h"
#include "../centralwidget.h"
#include "Matches/matchesbasicwidget.h"

class CentralWidget;
class TextBasicWidget;
class PatternsBasicWidget;
class TopMenu : public QMenuBar
{
    Q_OBJECT
private:
    QMenu* fileMenu;
    QMenu* patternsMenu;
    QMenu* matchesMenu;
    QMenu* windowMenu;
    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* saveAllAct;
    QAction* exitAct;
    QAction* exportMatchesAct;
    QAction* importPatternsAct;
    QAction* exportPatternsAct;
    QAction* clearPatternsAct;
    void setFileMenu();
    void setFileActions();
    void setPatternMenu();
    void setPatternActions();
    void setMatchesMenu();
    void setMatchesAction();
    TextBasicWidget* text;
    PatternsBasicWidget* pattern;
    MatchesBasicWidget* matches;
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

    void importPatterns();
    void exportPatterns();
    void clearPatterns();

    void exportMatches();
};

#endif // TOPMENU_H
