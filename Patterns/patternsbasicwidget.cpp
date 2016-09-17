#include "patternsbasicwidget.h"
#include <QDebug>

void PatternsBasicWidget::initPatternEditor()
{
    QMainWindow* wrapper = new QMainWindow();
    QToolBar* buttonBar = new QToolBar(this);

    buttonBar->setMovable(false);
    buttonBar->setFloatable(false);
    compileButton = new QPushButton(tr("Компиляция"),this);
    matchButton = new QPushButton(tr("Сопоставление"), this);
    clearButton= new QPushButton(tr("Удалить"),this);
    addButton = new QPushButton(tr("Добавить"),this);
    selectAllButton = new QPushButton(tr("Выбрать всё"),this);
    deselectAllButton = new QPushButton(tr("Сбросить всё"),this);
    buttonBar->addWidget(compileButton);
    buttonBar->addWidget(matchButton);
    buttonBar->addWidget(selectAllButton);
    buttonBar->addWidget(deselectAllButton);
    buttonBar->addWidget(clearButton);
    QWidget* container =new QWidget(this);
    QVBoxLayout* lay = new QVBoxLayout(container);
    lay->setContentsMargins(0,0,0,0);
    list = new PatternsList(comp,this);
    editor = new PatternEditor(this);
    connect(compileButton,SIGNAL(clicked(bool)),list,SLOT(slotCompilePatterns()));
    connect(clearButton,SIGNAL(clicked(bool)),this,SLOT(slotDeleteSelected()));
    connect(matchButton,SIGNAL(clicked(bool)),this,SLOT(slotMatchClicked()));
    connect(addButton,SIGNAL(clicked(bool)),this,SLOT(slotAddPattern()));
    connect(selectAllButton,SIGNAL(clicked(bool)),list,SLOT(slotSelectAll()));
    connect(deselectAllButton,SIGNAL(clicked(bool)),list,SLOT(slotDeselectAll()));
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

void PatternsBasicWidget::slotEnableMatch()
{
    matchButton->setEnabled(true);
}

void PatternsBasicWidget::slotMatchClicked()
{
    matchButton->setDisabled(true);
    emit matchClicked();
}

void PatternsBasicWidget::slotDisableMatch()
{
    matchButton->setDisabled(true);
}

void PatternsBasicWidget::slotDeleteSelected()
{
    QStringList selected = list->getSelectedPatternNames();
    if(!selected.empty()) {
        QMessageBox msgBox(
                    QMessageBox::Information,
                    tr("Удалить выбранные шаблоны?"),
                   "Данные шаблоны будут удалены: "+ selected.join(',')+ ".",
                    QMessageBox::Yes | QMessageBox::No
         );
        msgBox.setButtonText(QMessageBox::Yes, tr("Да"));
        msgBox.setButtonText(QMessageBox::No, tr("Нет"));

        if (msgBox.exec() == QMessageBox::Yes) {
            list->slotDeleteChecked();
        }
    }
}






