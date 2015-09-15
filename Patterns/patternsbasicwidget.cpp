#include "patternsbasicwidget.h"
#include <QDebug>
void PatternsBasicWidget::initCompileButton()
{
    comp = new PatternCompiler();
    compileButton = new QPushButton("Compile",this);
    connect(compileButton,SIGNAL(clicked(bool)),this,SLOT(compilePattern()));

}

void PatternsBasicWidget::initPatternTable()
{
    tableModel = new PatternCompiledTable();
    table = new QTableView();
    table->setModel(tableModel);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    for(int c = 0; c < table->horizontalHeader()->count();++c){
        table->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
}

void PatternsBasicWidget::initPatternEditor()
{
    editorTab = new PatternEditorTab(this);
}

void PatternsBasicWidget::initPatternLogBar()
{
    logBar = new PatternCompLogBar();
    logtab = new QTabWidget(this);
    logtab->addTab(logBar,"Compilation output");
}

PatternsBasicWidget::PatternsBasicWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout * lay = new QGridLayout(this);

    initCompileButton();
    initPatternEditor();
    initPatternTable();
    initPatternLogBar();
    lay->addWidget(compileButton,0,0);
    lay->addWidget(editorTab,1,0);
    lay->addWidget(table,1,1);
    lay->addWidget(logtab,2,0,1,2);
}

PatternsBasicWidget::~PatternsBasicWidget()
{
    delete comp;
}

void PatternsBasicWidget::compilePattern()
{
    QString patternText = editorTab->getText();
    QStringList patterns = patternText.split("\n");
    logBar->setText("");
    for(QString pattern: patterns){
        QString patternName = pattern.split("=").at(0);
        try{
            comp->compilePattern(pattern.toStdString().c_str());
            tableModel->addPattern(patternName);
            logBar->append("Pattern " + patternName + " successfully compiled.");
        }catch(lspl::patterns::PatternBuildingException e){
            logBar->append("Error in pattern: " + patternName);
            logBar->append("\t"+QString(e.what()));
        }
    }
}

