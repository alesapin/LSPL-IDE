#include "texttabedit.h"
#include <QDebug>
TextTabEdit::TextTabEdit(QWidget* parent):QTabWidget(parent)
{
    par = static_cast<TextBasicWidget*>(parent);
    this->setTabsClosable(true);
    setMinimumHeight(300);
    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(slotCloseTab(int)));

}

QString TextTabEdit::addAnotherTab(const QString &filename, const QString &text)
{
    QString name = filename.split("/").last();
    MainTextViewer* current = new MainTextViewer();
    fileNamePath[name] = filename;
    current->setPlainText(text);
    addTab(current,name);
    setCurrentWidget(current);
    connect(current,SIGNAL(jobDone()),this,SIGNAL(checkingEnabled()));
    return name;
}

QString TextTabEdit::getCurrentText() const
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    if(current){
        return current->toPlainText();
    } else {
        return "";
    }
}

QString TextTabEdit::getCurrentFile() const
{
    int index = currentIndex();
    return fileNamePath[tabText(index)];
}

QString TextTabEdit::getIndexText(int index) const
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->widget(index));
    if (current) {
        return current->toPlainText();
    }
    return "";
}

void TextTabEdit::highLightPatterns(const QStringList &patterns)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    if(current){
        current->highlightPatterns(patterns);
    }
}

void TextTabEdit::deHighlightPattern(const QString &pattern)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    if(current){
        current->dehighlightPattern(pattern);
    }
}


void TextTabEdit::setMatches(QSharedPointer<utility::IntervalViewMap> m)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    if(current){
        current->setMatches(m);
    }
}

QSharedPointer<utility::IntervalViewMap> TextTabEdit::getMatches() const
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    if(current){
        return current->getMatches();
    }
    return QSharedPointer<utility::IntervalViewMap>(new utility::IntervalViewMap());
}

void TextTabEdit::selectText(int start, int end)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    if(current){
        current->selectText(start,end);
    }
}

void TextTabEdit::clearSelection()
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    if(current){
        current->stopCalcing();
        current->dehighlightAll();
    }
}

void TextTabEdit::setReadOnly(bool f)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    if(current){
        if(f){
            current->setStyleSheet("QPlainTextEdit {background-color: rgb(0, 255, 0,20%)}");
        }else{
            current->setStyleSheet("QPlainTextEdit {background-color: rgb(255, 255, 255)}");
            current->setMatches(QSharedPointer<utility::IntervalViewMap>(new utility::IntervalViewMap));
        }
        current->setReadOnly(f);
    }
}

void TextTabEdit::dehighlightAll()
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    current->dehighlightAll();
}

void TextTabEdit::highlightAll()
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    current->highlightAll();
}

void TextTabEdit::slotCloseTab(int index)
{
        MainTextViewer* current = static_cast<MainTextViewer*>(this->widget(index));
        if(current->isModified()){
            QString name = tabText(index);
            if(par->maybeSave(name,index)){
                this->removeTab(index);
                emit tabWasClosed(index);
            }
        }else{
            this->removeTab(index);
            emit tabWasClosed(index);
        }
}

void TextTabEdit::slotRenameCurrentTab(const QString &filename)
{
    QString name = filename.split("/").last();
    fileNamePath[name] = filename;
    setTabText(this->currentIndex(),name);
}

