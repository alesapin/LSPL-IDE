#include "matcheswidget.h"





MatchesWidget::MatchesWidget(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* lay = new QVBoxLayout(this);
    list = new PatternSelectionList(this);
    table = new TextMatchesTable(this);
    lay->addWidget(list);
    lay->addWidget(table);
    connect(table,SIGNAL(rowClicked(int,int)),this,SLOT(slotTransferSignal(int,int)));
    connect(list,SIGNAL(patternUncheked(QString)),this,SLOT(slotPatternUnchecked(QString)));
    connect(list,SIGNAL(patternChecked(QString)),this,SLOT(slotPatternChecked(QString)));
}

void MatchesWidget::setPatterns(const PatternViewMap &patterns)
{
    table->setMatches(patterns);
    list->addItems(patterns.keys());
    list->setCheckedItems(patterns.keys());
}



void MatchesWidget::slotTransferSignal(int s, int e)
{
    emit rowClicked(s,e);
}

void MatchesWidget::slotPatternUnchecked(const QString &s)
{
    table->removePatternFromCurrent(s);
}

void MatchesWidget::slotPatternChecked(const QString &s)
{
    table->addCurrentPattern(s);
}
