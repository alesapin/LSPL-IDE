#include "texttabedit.h"
#include <QDebug>
MatchTextViewer *TextTabEdit::getCurrentWidget() const
{
    QTabWidget* currentTab =  static_cast<QTabWidget*>(this->currentWidget());
    return static_cast<MatchTextViewer*>(currentTab->widget(0));
}

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
    QTabWidget* viewerTab = new QTabWidget(this);

    MatchTextViewer* current = new MatchTextViewer(viewerTab);
    MorphologyViewer* currentMorph = new MorphologyViewer(viewerTab);
    viewerTab->setTabPosition(QTabWidget::South);
    viewerTab->addTab(current,"Text");
    viewerTab->addTab(currentMorph,"Morphology");
    fileNamePath[name] = filename;
    current->setPlainText(text);
    currentMorph->setPlainText(text);
    addTab(viewerTab,name);
    setCurrentWidget(current);
    connect(current,SIGNAL(jobDone()),this,SIGNAL(checkingEnabled()));
    return name;
}

QString TextTabEdit::getCurrentText() const
{
    MatchTextViewer* current = getCurrentWidget();
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
    MatchTextViewer* current = getCurrentWidget();
    if (current) {
        return current->toPlainText();
    }
    return "";
}

void TextTabEdit::highLightPatterns(const QStringList &patterns)
{
    MatchTextViewer* current = getCurrentWidget();
    if(current){
        current->highlightPatterns(patterns);
    }
}

void TextTabEdit::deHighlightPattern(const QString &pattern)
{
    MatchTextViewer* current = getCurrentWidget();
    if(current){
        current->dehighlightPattern(pattern);
    }
}


void TextTabEdit::setMatches(QSharedPointer<utility::IntervalViewMap> m)
{
    MatchTextViewer* current = getCurrentWidget();
    if(current){
        current->setMatches(m);
    }
}

QSharedPointer<utility::IntervalViewMap> TextTabEdit::getMatches() const
{
    MatchTextViewer* current = getCurrentWidget();
    if(current){
        return current->getMatches();
    }
    return QSharedPointer<utility::IntervalViewMap>(new utility::IntervalViewMap());
}

void TextTabEdit::selectText(int start, int end)
{
    MatchTextViewer* current = getCurrentWidget();
    if(current){
        current->selectText(start,end);
    }
}

void TextTabEdit::clearSelection()
{
    MatchTextViewer* current = getCurrentWidget();
    if(current){
        current->stopCalcing();
        current->dehighlightAll();
    }
}

void TextTabEdit::setReadOnly(bool f)
{
    MatchTextViewer* current = getCurrentWidget();
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
    MatchTextViewer* current = getCurrentWidget();
    current->dehighlightAll();
}

void TextTabEdit::highlightAll()
{
    MatchTextViewer* current = getCurrentWidget();
    current->highlightAll();
}

void TextTabEdit::slotCloseTab(int index)
{
        MatchTextViewer* current = getCurrentWidget();
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

