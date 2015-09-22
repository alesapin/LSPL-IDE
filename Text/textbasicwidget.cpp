#include "textbasicwidget.h"
void TextBasicWidget::saveTextFile(QString filename,int index)
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
       if(index == -1){
            out << textEdit->getCurrentText();
       }else{
           out << textEdit->getIndexText(index);
       }
   #ifndef QT_NO_CURSOR
       QApplication::restoreOverrideCursor();
#endif
}

void TextBasicWidget::openTextFile(const QString &filename)
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
    textEdit->addAnotherTab(filename,in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

bool TextBasicWidget::maybeSave(QString filename,int index)
{
    QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "Save", "Save file ?",
                                   QMessageBox::Yes| QMessageBox::Cancel |QMessageBox::No);
     if (reply == QMessageBox::Yes) {
         if(filename == "Untilted"){
             QFileDialog dialog(this);
             dialog.setWindowModality(Qt::WindowModal);
             dialog.setAcceptMode(QFileDialog::AcceptSave);
             QStringList files;
             if (dialog.exec()) {
                     files = dialog.selectedFiles();
             }
             filename = files.at(0);
         }
         saveTextFile(filename,index);
         return true;
       } else if(reply == QMessageBox::No){
          return true;
       } else {
         return false;
     }
}

void TextBasicWidget::newTextFile()
{
    textEdit->addAnotherTab("Untilted","");
}

void TextBasicWidget::setMatches(const PatternViewMap &matches)
{
    this->matches->setMatches(matches);
    this->textEdit->setMatches(matches);
}

void TextBasicWidget::highlighPatterns(const QStringList &patterns)
{
    textEdit->highLightPatterns(patterns);
}

void TextBasicWidget::dehighlightPatterns(const QStringList &patterns)
{
    textEdit->deHighlightPatterns(patterns);
}

QString TextBasicWidget::getCurrentFile() const
{
    return textEdit->getCurrentFile();
}


QString TextBasicWidget::getText() const
{
    return textEdit->getCurrentText();
}
/**
 * @brief TextBasicWidget::analyzeText
 * Излучает сигнал наружу, единственная кнопка
 * которая связывает 2 большие группы виджетов.
 */
void TextBasicWidget::analyzeText()
{
//    CentralWidget* par = static_cast<CentralWidget*>(this->parent());
//    par->getChoosenPatterns();
    emit buttonClicked();
}

void TextBasicWidget::selectFragment(int from, int to)
{
    textEdit->selectText(from,to);
}

void TextBasicWidget::slotPatternUncheked(const QString &name)
{
    textEdit->deHighlightPatterns(QStringList() << name);
}

void TextBasicWidget::slotPatternChecked(const QString &name)
{
    textEdit->highLightPatterns(QStringList() << name);
}

void TextBasicWidget::slotTabChanged(int index)
{
    matches->changeTab(index);
}

void TextBasicWidget::initTable()
{
    matchTabs = new QTabWidget(this);
    matches = new MatchesWidget(this);
    matchTabs->addTab(matches,"Matches View");
    connect(matches,SIGNAL(rowClicked(int,int)),this,SLOT(selectFragment(int,int)));
    connect(matches,SIGNAL(patternWasChecked(QString)),this,SLOT(slotPatternChecked(QString)));
    connect(matches,SIGNAL(patternWasUnchecked(QString)),this,SLOT(slotPatternUncheked(QString)));
}

void TextBasicWidget::initEditor()
{
    textEdit = new TextTabEdit(this);
    connect(textEdit,SIGNAL(currentChanged(int)),this,SLOT(slotTabChanged(int)));
    connect(textEdit,SIGNAL(tabWasClosed(int)),matches,SLOT(closeTab(int)));
}

void TextBasicWidget::initButton()
{
    analyze = new QPushButton("Analyze",this);
    connect(analyze,SIGNAL(clicked(bool)),this,SLOT(analyzeText()));

}

TextBasicWidget::TextBasicWidget(PatternCompiler* comp,QWidget *parent) : QWidget(parent),compiler(comp)
{
    QHBoxLayout * lay = new QHBoxLayout(this);
    QSplitter* split = new QSplitter(this);
    initTable();
    initEditor();
    initButton();
    QWidget* container = new QWidget();
    QVBoxLayout* buttonEdit = new QVBoxLayout();
    buttonEdit->addWidget(analyze);
    buttonEdit->addWidget(textEdit);
    container->setLayout(buttonEdit);
    split->setOrientation(Qt::Vertical);

    split->addWidget(container);
    split->addWidget(matchTabs);
//    lay->addWidget(analyze,0,0);
//    lay->addWidget(textEdit,1,0);
//    lay->addWidget(matchTabs,2,0);
    lay->addWidget(split);
    this->setLayout(lay);
}

