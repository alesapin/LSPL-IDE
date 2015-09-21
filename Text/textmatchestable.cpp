#include "textmatchestable.h"

TextMatchesTable::TextMatchesTable(QWidget *parent) : QTableView(parent)
{
    model = new TextMatchesModel(this);
    setModel(model);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    for(int c = 0; c < horizontalHeader()->count();++c){
        horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    connect(this->selectionModel(),
                    SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                    this,
                    SLOT(onRowClick(QItemSelection,QItemSelection)));
}

void TextMatchesTable::setMatches(const PatternViewMap &maches)
{
    model->setMatches(maches);
    setCurrentPatterns(maches.keys());
}

void TextMatchesTable::setCurrentPatterns(const QStringList &name)
{
    model->setCurrentPatterns(name);
}

void TextMatchesTable::addCurrentPattern(const QString &name)
{
    model->addCurrentPattern(name);
}

void TextMatchesTable::removePatternFromCurrent(const QString &name)
{
    model->removePatternFromCurrent(name);
}

void TextMatchesTable::changeTab(int index)
{
    model->changeTab(index);
}

QStringList TextMatchesTable::getCurrentPatterns() const
{
    return model->getCurrentPatterns();
}

QStringList TextMatchesTable::getAllPatterns() const
{
    return model->getAllPatterns();
}

void TextMatchesTable::onRowClick(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(!selected.empty()){
        int row = selected.first().top();
        PatternCompiler::MatchRepr match = model->getRow(row);
        emit rowClicked(match.start,match.end);
    }
}

