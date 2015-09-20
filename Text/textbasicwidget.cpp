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
    textEdit->addAnotherTab(filename/*.split("/").last()*/,in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

bool TextBasicWidget::maybeSave(QString filename,int index)
{
    QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "Test", "Save file ?",
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
    this->matches->setPatterns(matches);



}

void TextBasicWidget::highlighMatches(const PatternViewMap &matches)
{
    QVector<QPair<int,int>> result;
    for(const QVector<PatternCompiler::MatchRepr>& match : matches.values()){
        for(int i = 0;i<match.size();++i){
            PatternCompiler::MatchRepr m = match[i];
            result.push_back(QPair<int,int>(m.start,m.end));
        }
    }
    textEdit->highLightText(result);
}

QString TextBasicWidget::getText()
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

void TextBasicWidget::initTable()
{
    matchTabs = new QTabWidget(this);
    matches = new MatchesWidget(this);
    matchTabs->addTab(matches,"Matches View");
    connect(matches,SIGNAL(rowClicked(int,int)),this,SLOT(selectFragment(int,int)));
}

void TextBasicWidget::initEditor()
{
    textEdit = new TextTabEdit(this);
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
    initEditor();
    initTable();
    initButton();
    split->setOrientation(Qt::Vertical);
    split->addWidget(analyze);
    split->addWidget(textEdit);
    split->addWidget(matchTabs);
//    lay->addWidget(analyze,0,0);
//    lay->addWidget(textEdit,1,0);
//    lay->addWidget(matchTabs,2,0);
    lay->addWidget(split);
    this->setLayout(lay);
}

