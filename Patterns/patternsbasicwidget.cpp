#include "patternsbasicwidget.h"
#include <QDebug>
void PatternsBasicWidget::initCompileButton()
{
    QToolBar* buttonBar = new QToolBar(this);
    compileButton = new QPushButton("Compile",this);
    connect(compileButton,SIGNAL(clicked(bool)),this,SLOT(compilePattern()));
    buttonBar->addWidget(compileButton);
    this->addToolBar(buttonBar);
}

void PatternsBasicWidget::initPatternTable()
{
//    tableModel = new PatternCompiledTable();
//    table = new QTableView();
//    table->setModel(tableModel);
//    table->setSelectionBehavior(QAbstractItemView::SelectRows);
////    for(int c = 0; c < table->horizontalHeader()->count();++c){
////        table->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
////    }
}

void PatternsBasicWidget::initPatternEditor()
{
    QDockWidget* wrapper = new QDockWidget("PatternEditor",this);
    wrapper->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    editor = new PatternEditor(this);
    wrapper->setWidget(editor);
    addDockWidget(Qt::RightDockWidgetArea, wrapper);


}

void PatternsBasicWidget::initPatternLogBar()
{
    QDockWidget* wrapper = new QDockWidget(this);
    wrapper->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    logBar = new PatternCompLogBar();
    logtab = new QTabWidget(this);
    logtab->addTab(logBar,"Compilation output");
    wrapper->setWidget(logtab);
    addDockWidget(Qt::RightDockWidgetArea,wrapper);
}

PatternsBasicWidget::PatternsBasicWidget(PatternCompiler* compiler,QWidget *parent) : QMainWindow(parent)
{
//    QSplitter *split2 = new QSplitter();
//    QSplitter *split3 = new QSplitter();

//    QHBoxLayout * lay = new QHBoxLayout(this);
    comp = compiler;
    initCompileButton();
    initPatternEditor();
//    initPatternTable();
    initPatternLogBar();
//    QVBoxLayout* buttonEdit = new QVBoxLayout();
//    buttonEdit->addWidget(compileButton);
//   // buttonEdit->addWidget(editor);
//    QWidget* container1 = new QWidget();
//    QWidget* container2 = new QWidget();
//    container1->setLayout(buttonEdit);
//    split2->addWidget(container1);
//    QVBoxLayout* containerLay = new QVBoxLayout();
//    containerLay->addWidget(split2);
//    container2->setLayout(containerLay);
//    split3->setOrientation(Qt::Vertical);
//    split3->addWidget(container2);
//    split3->addWidget(logtab);
//    lay->addWidget(split3);
}

QStringList PatternsBasicWidget::getChoosenPatterns()
{
    return editor->getCompiledPatterns();
}

void PatternsBasicWidget::setPatternValues(QString name, int segments, int matches, int variants)
{
    //tableModel->setPatternValues(name,segments,matches,variants);
}



void PatternsBasicWidget::compilePattern()
{
    QString patternText = editor->toPlainText();
    QStringList patterns = patternText.split("\n");
    logBar->setText("");
    for(QString pattern: patterns){
        QString patternName = pattern.split("=").at(0);
        QString res = comp->compilePattern(pattern);
        if(res.isEmpty()){
            editor->addPattern(patternName.simplified());
            logBar->append("Pattern " + patternName + " successfully compiled.");
        }else{
            logBar->append("Error in pattern: " + patternName.simplified());
            logBar->append("\t"+res);
        }
    }
}

