#include "matchesbasicwidget.h"

MatchesBasicWidget::MatchesBasicWidget(QWidget* parent) : BasicWidget(parent,"Сопоставления")
{
    QWidget* wrapper = new QWidget();
    QVBoxLayout* lay = new QVBoxLayout(wrapper);
    QHBoxLayout* line = new QHBoxLayout();
    selectPattern = new QLabel("Select Patterns: ");
    QSizePolicy small(QSizePolicy::Preferred,QSizePolicy::Preferred);
    small.setHorizontalStretch(0);
    selectPattern->setSizePolicy(small);
    QSizePolicy big(QSizePolicy::Preferred,QSizePolicy::Preferred);
    big.setHorizontalStretch(5);
    list = new PatternSelectionList(this);
    list->setSizePolicy(big);
    line->addWidget(selectPattern);
    line->addWidget(list);
    table = new MatchesTable(this);
    lay->addLayout(line);
    lay->addWidget(table);
    lay->setContentsMargins(0,0,0,0);
    wrapper->setLayout(lay);
    wrapper->setContentsMargins(10,10,10,10);
    setWidget(wrapper);
    connect(table,SIGNAL(rowClicked(int,int)),this,SLOT(slotTransferSignal(int,int)));
    connect(list,SIGNAL(patternUncheked(QString)),this,SLOT(slotPatternUnchecked(QString)));
    connect(list,SIGNAL(patternChecked(QString)),this,SLOT(slotPatternChecked(QString)));
}

void MatchesBasicWidget::setMatches(const PatternViewMap &patterns)
{
    table->setMatches(patterns);
    list->clear();
    list->addItems(patterns.keys());
    list->setCheckedItems(patterns.keys());
}

void MatchesBasicWidget::slotChangeTab(int index)
{
    if(index != -1){
        table->changeTab(index);
        list->clear();
        list->addItems(table->getAllPatterns());
        list->setCheckedItems(table->getCurrentPatterns());
    } else {
        table->clear();
        list->clearAll();
    }
}

void MatchesBasicWidget::saveMatches(QString filename)
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


PatternViewMap MatchesBasicWidget::getSelectedMatches() const
{
    return table->getCurrentMatches();
}

void MatchesBasicWidget::clear()
{
    table->clear();
    list->clearAll();
}

QDomDocument MatchesBasicWidget::getXml()
{
    return toXml(getSelectedMatches());

}

QDomDocument MatchesBasicWidget::toXml(PatternViewMap matches)
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

void MatchesBasicWidget::slotCloseTab(int index)
{
    table->closeTab(index);
    //changeTab(index+1);
}



void MatchesBasicWidget::slotTransferSignal(int s, int e)
{
    emit rowClicked(s,e);
}

void MatchesBasicWidget::slotPatternUnchecked(const QString &s)
{
    table->removePatternFromCurrent(s);
    emit patternWasUnchecked(s);
}

void MatchesBasicWidget::slotPatternChecked(const QString &s)
{
    table->addCurrentPattern(s);
    emit patternWasChecked(s);
}
