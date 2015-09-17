#include "patternsbasicwidget.h"
#include <QDebug>
void PatternsBasicWidget::initCompileButton()
{
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

PatternsBasicWidget::PatternsBasicWidget(PatternCompiler* compiler,QWidget *parent) : QWidget(parent)
{
    QSplitter *split1 = new QSplitter();
    QSplitter *split2 = new QSplitter();
    QSplitter *split3 = new QSplitter();
    split1->setOrientation(Qt::Vertical);

    QGridLayout * lay = new QGridLayout(this);
    comp = compiler;
    initCompileButton();
    initPatternEditor();
    initPatternTable();
    initPatternLogBar();
    split1->addWidget(compileButton);
    split1->addWidget(editorTab);
    split2->addWidget(split1);
    split2->addWidget(table);
    split3->setOrientation(Qt::Vertical);
    split3->addWidget(split2);
    split3->addWidget(logtab);
    lay->addWidget(split3);
}

QVector<QString> PatternsBasicWidget::getChoosenPatterns()
{
    return tableModel->getChoosenPatterns();
}



void PatternsBasicWidget::compilePattern()
{
    QString patternText = editorTab->getText();
    QStringList patterns = patternText.split("\n");
    logBar->setText("");
    for(QString pattern: patterns){
        QString patternName = pattern.split("=").at(0);
        QString res = comp->compilePattern(pattern);
        if(res.isEmpty()){
            tableModel->addPattern(patternName.simplified());
            logBar->append("Pattern " + patternName + " successfully compiled.");
        }else{
            logBar->append("Error in pattern: " + patternName.simplified());
            logBar->append("\t"+res);
        }
    }
}

