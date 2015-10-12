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

void TextBasicWidget::saveMatches(QString filename)
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
        out << getXml().toString();
   #ifndef QT_NO_CURSOR
       QApplication::restoreOverrideCursor();
#endif
}

QDomDocument TextBasicWidget::getXml()
{
    return toXml(matches->getSelectedMatches());
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

void TextBasicWidget::clearMatches()
{
    matches->clear();
    textEdit->clearSelection();
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
    QDockWidget* wrapper = new QDockWidget("Table View",this);
    wrapper->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    matchTabs = new QTabWidget(this);
    matchTabs->addTab(matches,"Matches View");
    connect(matches,SIGNAL(rowClicked(int,int)),this,SLOT(selectFragment(int,int)));
    connect(matches,SIGNAL(patternWasChecked(QString)),this,SLOT(slotPatternChecked(QString)));
    connect(matches,SIGNAL(patternWasUnchecked(QString)),this,SLOT(slotPatternUncheked(QString)));
    wrapper->setWidget(matchTabs);
    addDockWidget(Qt::LeftDockWidgetArea, wrapper);
}

void TextBasicWidget::initEditor()
{
    QDockWidget* wrapper = new QDockWidget("Text Editor",this);
    wrapper->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    textEdit = new TextTabEdit(this);
    wrapper->setWidget(textEdit);
    addDockWidget(Qt::LeftDockWidgetArea, wrapper);
    connect(textEdit,SIGNAL(currentChanged(int)),this,SLOT(slotTabChanged(int)));
    connect(textEdit,SIGNAL(tabWasClosed(int)),matches,SLOT(closeTab(int)));
}

void TextBasicWidget::initButton()
{
    QToolBar* buttonBar = new QToolBar(this);
    analyze = new QPushButton("Analyze",this);
    connect(analyze,SIGNAL(clicked(bool)),this,SLOT(analyzeText()));
    buttonBar->addWidget(analyze);
    this->addToolBar(buttonBar);

}

QDomDocument TextBasicWidget::toXml(PatternViewMap matches)
{
    QDomDocument doc;
    QDomElement header1 = doc.createElement("texts");
    QDomElement text = doc.createElement("text");
    for(QString pattern:matches.keys()){
        QDomElement ptrn = doc.createElement("goal");
        ptrn.setAttribute("name",pattern);
        for(PatternCompiler::MatchRepr m: matches[pattern]){
           QDomElement mtch = doc.createElement("match");
           mtch.setAttribute("startPos",m.start);
           mtch.setAttribute("endPos",m.end);
           QDomElement frag = doc.createElement("fragment");
           QDomText text = doc.createTextNode(m.text);
           frag.appendChild(text);
           mtch.appendChild(frag);
           if(!m.transform.isEmpty()){
                QDomElement res = doc.createElement("result");
                QDomText trns = doc.createTextNode(m.transform);
                res.appendChild(trns);
                mtch.appendChild(res);
           }
           ptrn.appendChild(mtch);
        }
        text.appendChild(ptrn);
    }
    header1.appendChild(text);
    doc.appendChild(header1);
    return doc;
}

TextBasicWidget::TextBasicWidget(PatternCompiler* comp,QWidget *parent) : QMainWindow(parent),compiler(comp)
{
//    QHBoxLayout * lay = new QHBoxLayout(this);
//    QSplitter* split = new QSplitter(this);
//    this->setStyleSheet("QDockWidget > QWidget { \
//                                                border: 2px solid red;\
//                                                border-top: 0px ; \
//                                                padding: 5px; \
//                                              }\
//                                            QDockWidget::title{ \
//                                                border: 2px solid red;\
//                                                border-bottom: 0px;\
//                                                padding: 5px;\
//                                            }\
//                                            QTabWidget {\
//                                                border:2px solid purple;\
//                                                }");
    initButton();
    matches = new MatchesWidget(this);
    initEditor();
    initTable();
//    QWidget* container = new QWidget();
//    QVBoxLayout* buttonEdit = new QVBoxLayout();
//    buttonEdit->addWidget(analyze);
//    buttonEdit->addWidget(textEdit);
//    container->setLayout(buttonEdit);
//    split->setOrientation(Qt::Vertical);

//    split->addWidget(container);
//    split->addWidget(matchTabs);
////    lay->addWidget(analyze,0,0);
////    lay->addWidget(textEdit,1,0);
////    lay->addWidget(matchTabs,2,0);
//    lay->addWidget(split);
//    this->setLayout(lay);
}

