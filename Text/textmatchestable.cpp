#include "textmatchestable.h"

TextMatchesTable::TextMatchesTable(QWidget *parent) : QTableView(parent)
{
    model = new TextMatchesModel(this);
    setModel(model);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    horizontalHeader()->setStretchLastSection(true);
    connect(this->selectionModel(),
                    SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                    this,
                    SLOT(onRowClick(QItemSelection,QItemSelection)));
    setStyleSheet("QToolTip { color: #ffffff; background-color: #696969; border: 1px solid white; }");
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

void TextMatchesTable::closeTab(int index)
{
    model->closeTab(index);
}

QStringList TextMatchesTable::getCurrentPatterns() const
{
    return model->getCurrentPatterns();
}

QStringList TextMatchesTable::getAllPatterns() const
{
    return model->getAllPatterns();
}

PatternViewMap TextMatchesTable::getCurrentMatches() const
{
    return model->getCurrentMatches();
}

void TextMatchesTable::clear()
{
    model->clearTable();
    model->clearCurrent();
    model->clearDatum();
}

void TextMatchesTable::resizeEvent(QResizeEvent *event)
{
    if(event->oldSize().width() < event->size().width()){
        setColumnWidth(0, this->width()/3);
        setColumnWidth(1, this->width()/3);
        setColumnWidth(2, this->width()/3);
    }

    QTableView::resizeEvent(event);
}

void TextMatchesTable::onRowClick(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(!selected.empty()){
        int row = selected.first().top();
        PatternCompiler::MatchRepr match = model->getRow(row);
        emit rowClicked(match.start,match.end);
    }
}

