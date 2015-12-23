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
#include <QSplitter>
#include <QDockWidget>
#include <QToolBar>
#include <QFrame>
#include <QScrollArea>
#include "basicwidget.h"
#include "patternslist.h"
class PatternsBasicWidget : public BasicWidget
{
    Q_OBJECT
private:
    PatternEditor* editor;
    QPushButton* compileButton;
    QPushButton* addButton;
    PatternCompiler* comp;
    QTabWidget* logtab;
    PatternsList* list;
    void initCompileButton();
    void initPatternTable();
    void initPatternEditor();
    void initPatternLogBar();
    void clearDuplicates(QStringList& listPatterns,QStringList& editorPatterns);
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
    void slotClearPatterns();
    void slotAddPattern();
};

#endif // PATTERNSBASICWIDGET_H
