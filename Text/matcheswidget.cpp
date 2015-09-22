#include "matcheswidget.h"





MatchesWidget::MatchesWidget(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* lay = new QVBoxLayout(this);
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
    table = new TextMatchesTable(this);
    lay->addLayout(line);
    lay->addWidget(table);
    connect(table,SIGNAL(rowClicked(int,int)),this,SLOT(slotTransferSignal(int,int)));
    connect(list,SIGNAL(patternUncheked(QString)),this,SLOT(slotPatternUnchecked(QString)));
    connect(list,SIGNAL(patternChecked(QString)),this,SLOT(slotPatternChecked(QString)));
}

void MatchesWidget::setMatches(const PatternViewMap &patterns)
{
    table->setMatches(patterns);
    list->clear();
    list->addItems(patterns.keys());
    list->setCheckedItems(patterns.keys());
}

void MatchesWidget::changeTab(int index)
{
    table->changeTab(index);
    list->clear();
    list->addItems(table->getAllPatterns());
    list->setCheckedItems(table->getCurrentPatterns());
}

void MatchesWidget::closeTab(int index)
{
    table->closeTab(index);
}



void MatchesWidget::slotTransferSignal(int s, int e)
{
    emit rowClicked(s,e);
}

void MatchesWidget::slotPatternUnchecked(const QString &s)
{
    table->removePatternFromCurrent(s);
    emit patternWasUnchecked(s);
}

void MatchesWidget::slotPatternChecked(const QString &s)
{
    table->addCurrentPattern(s);
    emit patternWasChecked(s);
}
