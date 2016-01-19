#include "patternsbasicwidget.h"
#include <QDebug>
void PatternsBasicWidget::initCompileButton()
{
//    QToolBar* buttonBar = new QToolBar(this);
//    compileButton = new QPushButton("Compile",this);
//    connect(compileButton,SIGNAL(clicked(bool)),this,SLOT(compilePattern()));
//    buttonBar->addWidget(compileButton);
//    this->addToolBar(buttonBar);
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
    QMainWindow* wrapper = new QMainWindow();
    QToolBar* buttonBar = new QToolBar(this);
    buttonBar->setMovable(false);
    compileButton = new QPushButton("Compile",this);
    addButton = new QPushButton("Add",this);
    buttonBar->addWidget(compileButton);
    QWidget* container =new QWidget(this);
    QVBoxLayout* lay = new QVBoxLayout(container);
    lay->setContentsMargins(0,0,0,0);
    list = new PatternsList(comp,this);
    editor = new PatternEditor(this);
    connect(compileButton,SIGNAL(clicked(bool)),list,SLOT(slotCompilePatterns()));
    connect(addButton,SIGNAL(clicked(bool)),this,SLOT(slotAddPattern()));
    connect(list,SIGNAL(editPatternSignal(QString)),editor,SLOT(setText(QString)));
    QHBoxLayout* line = new QHBoxLayout();
    line->addWidget(editor);
    line->addWidget(addButton);

    lay->addWidget(list);
    lay->addLayout(line);
    lay->setStretch(0,5);
    lay->setStretch(1,1);
    container->setLayout(lay);
    wrapper->setContentsMargins(10,10,10,10);
    wrapper->setCentralWidget(container);
    wrapper->addToolBar(buttonBar);
    setWidget(wrapper);



}

void PatternsBasicWidget::initPatternLogBar()
{
//    QDockWidget* wrapper = new QDockWidget(this);
//    wrapper->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//    logBar = new PatternCompLogBar();
//    logtab = new QTabWidget(this);
//    logtab->addTab(logBar,"Compilation output");
////    QSizePolicy small(QSizePolicy::Expanding,QSizePolicy::Expanding);
////    small.setVerticalStretch(1);
////    logtab->setSizePolicy(small);

//    wrapper->setWidget(logtab);
//    addDockWidget(Qt::RightDockWidgetArea,wrapper);
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

PatternsBasicWidget::PatternsBasicWidget(PatternCompiler* compiler,QWidget *parent) : BasicWidget(parent,"Шаблоны")
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
    //initPatternLogBar();
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
    list->addPatterns(in.readAll().split('\n'));
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
        out << list->getCompiledPatterns().join('\n');
   #ifndef QT_NO_CURSOR
       QApplication::restoreOverrideCursor();
#endif
}

void PatternsBasicWidget::addLog(const QString &text)
{
    //logBar->append(text);
}

void PatternsBasicWidget::slotClearPatterns()
{
    comp->clear();
    editor->clearAll();
    list->clearAll();
}

void PatternsBasicWidget::slotAddPattern()
{
    list->addPattern(editor->getText());
    editor->clean();
}



void PatternsBasicWidget::compilePattern()
{
//    QStringList patterns = editor->getPatternsForCompile();
//    QStringList recompiledPatterns =  list->getPatternsForCompile();
//    clearDuplicates(recompiledPatterns,patterns);
//    recompiledPatterns << patterns;
//    comp->clear();
//    QStringList compiledPatterns;
//    for(QString pattern: recompiledPatterns){
//        qDebug() << pattern;
//        QString patternName = pattern.split("=").at(0);
//        QString res = comp->compilePatternNoException(pattern);
//        if(res.isEmpty()){
//            compiledPatterns << pattern;
//            //logBar->append("Pattern " + patternName + " successfully compiled.");
//        }else{
//            //logBar->append("Error in pattern: " + patternName.simplified());
//            //logBar->append("\t"+res);
//        }
//    }

//    editor->clear();
//    list->addPatterns(compiledPatterns);
}

