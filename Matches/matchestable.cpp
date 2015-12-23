#include "matchestable.h"

MatchesTable::MatchesTable(QWidget *parent) : QTableView(parent)
{
    setModel(new MatchesModel(this));
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    horizontalHeader()->setStretchLastSection(true);
    connect(this->selectionModel(),
                    SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                    this,
                    SLOT(onRowClick(QItemSelection,QItemSelection)));
    setStyleSheet("QToolTip { color: #ffffff; background-color: #696969; border: 1px solid white; }");
}

void MatchesTable::setMatches(const PatternViewMap &maches)
{
    myModel()->setMatches(maches);
    setCurrentPatterns(maches.keys());
}

void MatchesTable::setCurrentPatterns(const QStringList &name)
{
    myModel()->setCurrentPatterns(name);
}

void MatchesTable::addCurrentPattern(const QString &name)
{
    myModel()->addCurrentPattern(name);
}

void MatchesTable::removePatternFromCurrent(const QString &name)
{
    myModel()->removePatternFromCurrent(name);
}

void MatchesTable::changeTab(int index)
{
    myModel()->changeTab(index);
}

void MatchesTable::closeTab(int index)
{
    myModel()->closeTab(index);
}

QStringList MatchesTable::getCurrentPatterns() const
{
    return myModel()->getCurrentPatterns();
}

QStringList MatchesTable::getAllPatterns() const
{
    return myModel()->getAllPatterns();
}

PatternViewMap MatchesTable::getCurrentMatches() const
{
    return myModel()->getCurrentMatches();
}

void MatchesTable::clear()
{
    myModel()->clearTable();
    myModel()->clearCurrent();
    myModel()->clearDatum();
}

void MatchesTable::resizeEvent(QResizeEvent *event)
{
    if(event->oldSize().width() < event->size().width()){
        setColumnWidth(0, this->width()/3);
        setColumnWidth(1, this->width()/3);
        setColumnWidth(2, this->width()/3);
    }

    QTableView::resizeEvent(event);
}

void MatchesTable::onRowClick(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(!selected.empty()){
        int row = selected.first().top();
        PatternCompiler::MatchRepr match = myModel()->getRow(row);
        emit rowClicked(match.start,match.end);
    }
}

