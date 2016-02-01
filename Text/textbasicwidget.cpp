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
       textEdit->slotRenameCurrentTab(filename);
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
    in.setCodec(utility::codecNameForText(file));

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

void TextBasicWidget::setMatches(QSharedPointer<utility::IntervalViewMap> matches)
{
    textEdit->setReadOnly(true);
    textEdit->setMatches(matches);
}

void TextBasicWidget::setReadOnly(bool)
{
    textEdit->setReadOnly(true);
}

QString TextBasicWidget::getCurrentFile() const
{
    return textEdit->getCurrentFile();
}


QString TextBasicWidget::getText() const
{
    return textEdit->getCurrentText();
}

void TextBasicWidget::slotClearMatches()
{
    textEdit->clearSelection();
}

void TextBasicWidget::slotAnalyzeText(){
    emit buttonClicked();
}

void TextBasicWidget::slotEditEnable()
{
    textEdit->clearSelection();
    textEdit->setReadOnly(false);
    emit editEnabled();
}

void TextBasicWidget::slotShowStatistics()
{
    StatisticsWindow* w = new StatisticsWindow(textEdit->getMatches(),this);
    w->show();
}

void TextBasicWidget::slotSelectFragment(int from, int to)
{
    textEdit->selectText(from,to);
}

void TextBasicWidget::slotPatternUncheked(const QString &name)
{
    textEdit->deHighlightPattern(name);
}

void TextBasicWidget::slotPatternChecked(const QString &name)
{
    textEdit->highLightPatterns(QStringList() << name);
}

void TextBasicWidget::slotHighlightAll()
{
    textEdit->highlightAll();
}

void TextBasicWidget::slotDehighlightAll()
{
    textEdit->dehighlightAll();
}

void TextBasicWidget::initEditor( QMainWindow* wrapper)
{
    textEdit = new TextTabEdit(wrapper);
    connect(textEdit,SIGNAL(currentChanged(int)),this,SIGNAL(tabChanged(int)));
    connect(textEdit,SIGNAL(tabWasClosed(int)),this,SIGNAL(tabClosed(int)));
    wrapper->setCentralWidget(textEdit);
}

void TextBasicWidget::initButtons(QMainWindow* wrapper)
{
    QToolBar* buttonBar = new QToolBar(this);
    buttonBar->setMovable(false);
    buttonBar->setFloatable(false);
    analyze = new QPushButton("Analyze",this);
    edit = new QPushButton("Edit",this);
    statistics = new QPushButton("Statistics",this);
    connect(analyze,SIGNAL(clicked(bool)),this,SLOT(slotAnalyzeText()));
    connect(edit,SIGNAL(clicked(bool)),this,SLOT(slotEditEnable()));
    connect(statistics,SIGNAL(clicked(bool)),this,SLOT(slotShowStatistics()));
    buttonBar->addWidget(analyze);
    buttonBar->addWidget(edit);
    buttonBar->addWidget(statistics);

    wrapper->addToolBar(buttonBar);

}


TextBasicWidget::TextBasicWidget(QWidget *parent) : BasicWidget(parent,"Текст")
{
    QMainWindow* wrapper = new QMainWindow();
    wrapper->setContentsMargins(10,0,10,10);
    initButtons(wrapper);
    initEditor(wrapper);
    setWidget(wrapper);
    connect(textEdit,SIGNAL(checkingEnabled()),this,SIGNAL(checkingEnabled()));
}

