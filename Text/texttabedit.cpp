#include "texttabedit.h"
#include <QDebug>
TextTabEdit::TextTabEdit(QWidget* parent):QTabWidget(parent)
{

    this->setTabsClosable(true);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding );
    setMinimumHeight(300);
//    setMinimumWidth(300);
    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));

}

QString TextTabEdit::addAnotherTab(const QString &filename, const QString &text)
{
    QString name = filename.split("/").last();
    MainTextViewer* current = new MainTextViewer();
    fileNamePath[name] = filename;
    current->setPlainText(text);
    addTab(current,name);
    setCurrentWidget(current);
    return name;
}

QString TextTabEdit::getCurrentText() const
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    return current->toPlainText();
}

QString TextTabEdit::getCurrentFile() const
{
    int index = currentIndex();
    return fileNamePath[tabText(index)];
}

QString TextTabEdit::getIndexText(int index) const
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->widget(index));
    return current->toPlainText();
}

void TextTabEdit::highLightPatterns(const QStringList &patterns)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    current->highlightPatterns(patterns);
}

void TextTabEdit::deHighlightPatterns(const QStringList &patterns)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    current->dehighlightPatterns(patterns);
}

//void TextTabEdit::highLightText(const QVector<QPair<int, int> > &frags)
//{
//    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
//    current->highlightFragments(frags);
//}

void TextTabEdit::setMatches(const PatternViewMap &m)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    current->setMatches(m);
}

void TextTabEdit::selectText(int start, int end)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    current->selectText(start,end);
}

void TextTabEdit::closeTab(int index)
{
        MainTextViewer* current = static_cast<MainTextViewer*>(this->widget(index));
        if(current->isModified()){
            QString name = tabText(index);
            TextBasicWidget* par = static_cast<TextBasicWidget*>(this->parent());
            if(par->maybeSave(name,index)){
                this->removeTab(index);
                emit tabWasClosed(index);
            }
        }else{
            this->removeTab(index);
            emit tabWasClosed(index);
        }
}

