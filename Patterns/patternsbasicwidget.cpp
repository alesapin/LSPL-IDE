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
    QWidget* container =new QWidget(this);
    QVBoxLayout* lay = new QVBoxLayout(container);
    list = new PatternCompiledList(this);
    editor = new PatternEditor(this);
    editor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QScrollArea* ar = new QScrollArea();
    //QSizePolicy listPolicy(QSizePolicy::Expanding,QSizePolicy::qt_static_metacall());
    //list->setSizePolicy(listPolicy);
    list->setMinimumHeight(0);
    lay->addWidget(list);
    lay->addWidget(editor);
    container->setLayout(lay);
    ar->setWidget(container);
    ar->setWidgetResizable(true);
    ar->setContentsMargins(50,50,50,50);
//    QTabWidget* editorTab = new QTabWidget();
//    editorTab->addTab(editor,"Pattern Editor");
//    QSizePolicy large(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    large.setVerticalStretch(5);
//    editor->setSizePolicy(large);
    wrapper->setWidget(ar);

    addDockWidget(Qt::RightDockWidgetArea, wrapper);


}

void PatternsBasicWidget::initPatternLogBar()
{
    QDockWidget* wrapper = new QDockWidget(this);
    wrapper->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    logBar = new PatternCompLogBar();
    logtab = new QTabWidget(this);
    logtab->addTab(logBar,"Compilation output");
//    QSizePolicy small(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    small.setVerticalStretch(1);
//    logtab->setSizePolicy(small);

    wrapper->setWidget(logtab);
    addDockWidget(Qt::RightDockWidgetArea,wrapper);
}

void PatternsBasicWidget::clearDuplicates(QStringList &listPatterns, QStringList &editorPatterns)
{
    for(int i = 0; i < editorPatterns.size();++i){
        QString edPatName = editorPatterns[i].split("=")[0].trimmed();
        for(int j = 0;j<listPatterns.size();++j){
            QString lPatName = listPatterns[j].split("=")[0].trimmed();
            if(edPatName == lPatName) listPatterns.removeAt(j);
        }
    }
}

PatternsBasicWidget::PatternsBasicWidget(PatternCompiler* compiler,QWidget *parent) : QMainWindow(parent)
{
//    QSplitter *split2 = new QSplitter();
//    QSplitter *split3 = new QSplitter();

//    QHBoxLayout * lay = new QHBoxLayout(this);
//    this->setStyleSheet("QTabWidget::pane { /* The tab widget frame */ \
//                        border-top: 0px;\
//                    }");
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
    return list->getCompiledPatterns();
}

void PatternsBasicWidget::setPatternValues(QString name, int segments, int matches, int variants)
{
    //tableModel->setPatternValues(name,segments,matches,variants);
}

void PatternsBasicWidget::importPatterns(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(filename)
                                 .arg(file.errorString()));
            return;
       }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    list->setUncompiledPatterns(in.readAll().split('\n'));
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void PatternsBasicWidget::exportPatterns(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(filename)
                                 .arg(file.errorString()));
            return ;
     }
    QTextStream out(&file);
   #ifndef QT_NO_CURSOR
       QApplication::setOverrideCursor(Qt::WaitCursor);
   #endif
        out << list->getText();
   #ifndef QT_NO_CURSOR
       QApplication::restoreOverrideCursor();
#endif
}

void PatternsBasicWidget::addLog(const QString &text)
{
    logBar->append(text);
}

void PatternsBasicWidget::clearPatterns()
{
    comp->clear();
    editor->clearAll();
    list->clearAll();
    logBar->clear();
}



void PatternsBasicWidget::compilePattern()
{
    QStringList patterns = editor->getPatternsForCompile();
    QStringList recompiledPatterns =  list->getPatternsForCompile();
    clearDuplicates(recompiledPatterns,patterns);
    recompiledPatterns << patterns;
    comp->clear();
    QStringList compiledPatterns;
    for(QString pattern: recompiledPatterns){
        qDebug() << pattern;
        QString patternName = pattern.split("=").at(0);
        QString res = comp->compilePatternNoException(pattern);
        if(res.isEmpty()){
            compiledPatterns << pattern;
            logBar->append("Pattern " + patternName + " successfully compiled.");
        }else{
            logBar->append("Error in pattern: " + patternName.simplified());
            logBar->append("\t"+res);
        }
    }
    editor->clear();
    list->addPatterns(compiledPatterns);
}

