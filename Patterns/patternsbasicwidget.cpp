#include "patternsbasicwidget.h"
#include <QDebug>

void PatternsBasicWidget::initPatternEditor()
{
    QMainWindow* wrapper = new QMainWindow();
    QToolBar* buttonBar = new QToolBar(this);
    buttonBar->setMovable(false);
    buttonBar->setFloatable(false);
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
    wrapper->setContentsMargins(10,0,10,10);
    wrapper->setCentralWidget(container);
    wrapper->addToolBar(buttonBar);
    setWidget(wrapper);
}

PatternsBasicWidget::PatternsBasicWidget(PatternCompiler* compiler,QWidget *parent) : BasicWidget(parent,"Шаблоны")
{
    comp = compiler;
    initPatternEditor();
}

QStringList PatternsBasicWidget::getChoosenPatterns() const
{
    return list->getCompiledPatternsNames();
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
    const char* codecName = utility::codecNameForText(file);
    QTextStream in(&file);
    in.setCodec(codecName);
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





