#include "patternsbasicwidget.h"
#include <QDebug>

void PatternsBasicWidget::initPatternEditor()
{
    QMainWindow* wrapper = new QMainWindow();
    QToolBar* buttonBar = new QToolBar(this);

    buttonBar->setMovable(false);
    buttonBar->setFloatable(false);
    compileButton = new QPushButton(tr("Скомпилировать"),this);
    clearAllButton= new QPushButton(tr("Удалить всё"),this);
    addButton = new QPushButton(tr("Добавить"),this);
    buttonBar->addWidget(compileButton);
    buttonBar->addWidget(clearAllButton);
    QWidget* container =new QWidget(this);
    QVBoxLayout* lay = new QVBoxLayout(container);
    lay->setContentsMargins(0,0,0,0);
    list = new PatternsList(comp,this);
    editor = new PatternEditor(this);
    connect(compileButton,SIGNAL(clicked(bool)),list,SLOT(slotCompilePatterns()));
    connect(clearAllButton,SIGNAL(clicked(bool)),this,SLOT(slotClearPatterns()));
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

QStringList PatternsBasicWidget::getChoosenPatternsNames() const
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
        out << getPatternsAsText();
   #ifndef QT_NO_CURSOR
       QApplication::restoreOverrideCursor();
#endif
}

QString PatternsBasicWidget::getPatternsAsText() const
{
    return list->getCompiledPatterns().join('\n');
}

QStringList PatternsBasicWidget::getPatterns() const
{
    return list->getCompiledPatterns();
}

void PatternsBasicWidget::loadPatterns(const QStringList &patterns)
{
    list->clearAll();
    list->addPatterns(patterns);
}

void PatternsBasicWidget::slotClearPatterns()
{
    comp->clear();
    editor->clearAll();
    list->clearAll();
}

void PatternsBasicWidget::slotAddPattern()
{
    list->addPatterns(QStringList() << editor->getText());
    editor->clean();
}





