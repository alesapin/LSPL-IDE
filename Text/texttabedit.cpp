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

void TextTabEdit::addAnotherTab(const QString &filename, const QString &text)
{
    MainTextViewer* current = new MainTextViewer();
    current->setText(text);
    addTab(current,filename);
    setCurrentWidget(current);
}

QString TextTabEdit::getCurrentText() const
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    return current->toPlainText();
}

QString TextTabEdit::getIndexText(int index) const
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->widget(index));
    return current->toPlainText();
}

void TextTabEdit::highLightText(const QVector<QPair<int, int> > &frags)
{
    MainTextViewer* current = static_cast<MainTextViewer*>(this->currentWidget());
    current->highlightFragments(frags);
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
            }
        }else{
            this->removeTab(index);
        }
}

