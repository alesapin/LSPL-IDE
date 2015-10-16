#ifndef PATTERNSBASICWIDGET_H
#define PATTERNSBASICWIDGET_H

#include <QWidget>
#include <Patterns/patterneditor.h>
#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QCursor>
#include <QTextStream>
#include <QFileDialog>
#include <QStringList>
#include <QPushButton>
#include <Engine/patterncompiler.h>
#include <QTableView>
#include <QHeaderView>
#include <Patterns/patterncomplogbar.h>
#include <QSplitter>
#include <QDockWidget>
#include <QToolBar>
#include <QFrame>
#include "patterncompiledlist.h"
class PatternsBasicWidget : public QMainWindow
{
    Q_OBJECT
private:
    PatternEditor* editor;
    QPushButton* compileButton;
    PatternCompiler* comp;
    PatternCompLogBar* logBar;
    QTableView* table;
    QTabWidget* logtab;
    PatternCompiledList* list;
    void initCompileButton();
    void initPatternTable();
    void initPatternEditor();
    void initPatternLogBar();
public:
    explicit PatternsBasicWidget(PatternCompiler* compiler,QWidget *parent = 0);
    QStringList getChoosenPatterns();
    void setPatternValues(QString name,int segments,int matches,int variants);
    void importPatterns(QString filename);
    void exportPatterns(QString filename);
    void addLog(const QString& text);
signals:

public slots:
    void compilePattern();
    void clearPatterns();

};

#endif // PATTERNSBASICWIDGET_H
