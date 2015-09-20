#ifndef PATTERNSBASICWIDGET_H
#define PATTERNSBASICWIDGET_H

#include <QWidget>
#include <Patterns/patterneditortab.h>
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
#include <Patterns/patterncompiledtable.h>
#include <QTableView>
#include <QHeaderView>
#include <Patterns/patterncomplogbar.h>
#include <QSplitter>
class PatternsBasicWidget : public QWidget
{
    Q_OBJECT
private:
    PatternEditorTab* editorTab;
    QPushButton* compileButton;
    PatternCompiler* comp;
    PatternCompiledTable* tableModel;
    PatternCompLogBar* logBar;
    QTableView* table;
    QTabWidget* logtab;

    void initCompileButton();
    void initPatternTable();
    void initPatternEditor();
    void initPatternLogBar();
public:
    explicit PatternsBasicWidget(PatternCompiler* compiler,QWidget *parent = 0);
    QVector<QString> getChoosenPatterns();
    void setPatternValues(QString name,int segments,int matches,int variants);
signals:

public slots:
    void compilePattern();
};

#endif // PATTERNSBASICWIDGET_H
