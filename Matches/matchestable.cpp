#include "matchestable.h"

void MatchesTable::showOrHideAll(bool val)
{
    QModelIndex ri = rootIndex();
    for(int i = 0;i<myModel()->rowCount(ri);++i){
        QModelIndex current = myModel()->index(i,0,ri);
        for(int j = 0; j < myModel()->rowCount(current);++j){
            setRowHidden(j,current,val);
        }
        setRowHidden(i,ri,val);
    }
}

MatchesTable::MatchesTable(QWidget *parent) : QTreeView(parent), currentTab(0)
{
    models.append(new MatchesTreeModel(this));
    setModel(models[currentTab]);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    connect(this->selectionModel(),
                    SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                    this,
                    SLOT(slotOnRowClick(QItemSelection,QItemSelection)));
    setStyleSheet("QToolTip { color: #ffffff; background-color: #696969; border: 1px solid white; }");
}

void MatchesTable::setMatches(QSharedPointer<utility::IntervalViewMap> matches)
{
    myModel()->setMatches(matches);
}

void MatchesTable::patternVisibility(const QString &patternName, bool hide)
{
    QModelIndex ri = rootIndex();
    for(int i = 0;i<myModel()->rowCount(ri);++i){
        QStringList itemPatterns = myModel()->getRowPattern(i);
        if(itemPatterns.isEmpty()){
            continue;
        }else if(itemPatterns.size() == 1){
            if(itemPatterns[0] == patternName){
                setRowHidden(i,ri,hide);
            }
        }else {
            int ind = itemPatterns.indexOf(patternName);
            QModelIndex currentParent = myModel()->index(i,0,ri);
            while(ind != -1){
                setRowHidden(ind,currentParent,hide);
                ind = itemPatterns.indexOf(patternName,ind+1);
            }
            bool allHidden = true;
            for(int j = 0; j<myModel()->rowCount(currentParent);++j){
                if(!isRowHidden(j,currentParent)){
                    allHidden = false;
                    break;
                }
            }
            if(allHidden) setRowHidden(i,ri,true);
            else setRowHidden(i,ri,false);
        }
    }

}

void MatchesTable::hidePattern(const QString &patternName)
{
    patternVisibility(patternName,true);
}

void MatchesTable::showPattern(const QString &patternName)
{
    patternVisibility(patternName,false);
}


void MatchesTable::changeTab(int index)
{
    if(index < models.size()){
        currentTab = index;
        setModel(models[currentTab]);
        emit dataChanged(myModel()->index(0,0),myModel()->index(myModel()->rowCount(),myModel()->columnCount()));
    }else{
        models.append(new MatchesTreeModel(this));
        currentTab = models.size() - 1;
        setModel(models[currentTab]);
        emit dataChanged(myModel()->index(0,0),myModel()->index(myModel()->rowCount(),myModel()->columnCount()));
    }
}

void MatchesTable::closeTab(int index)
{
    if(index != -1){
        int newCurrent = index + 1;
        if(newCurrent < models.length()){
            setModel(models[newCurrent]);
            models.remove(index);
            currentTab = index;
        }else{
             newCurrent = index - 1;
             if(newCurrent >= 0){
                 setModel(models[newCurrent]);
                 models.remove(index);
                 currentTab = newCurrent;
             }else{
                 models[0]->clear();
                 currentTab = 0;
             }
        }
        emit dataChanged(myModel()->index(0,0),myModel()->index(myModel()->rowCount(),myModel()->columnCount()));
    }
}


QSharedPointer<utility::IntervalViewMap> MatchesTable::getCurrentMatches() const
{
    return myModel()->getMatches();
}

void MatchesTable::clear()
{
    myModel()->clear();
    emit dataChanged(myModel()->index(0,0),myModel()->index(myModel()->rowCount(),myModel()->columnCount()));
}

void MatchesTable::resizeEvent(QResizeEvent *event)
{
    if(event->oldSize().width() < event->size().width()){
        setColumnWidth(0, this->width()/3);
        setColumnWidth(1, this->width()/3);
        setColumnWidth(2, this->width()/3);
    }

    QTreeView::resizeEvent(event);
}

void MatchesTable::slotOnRowClick(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    if(!selected.empty()){
        QModelIndex cur = selected.first().indexes()[0];
        TreeItem* it = static_cast<TreeItem*>(cur.internalPointer());
        emit rowClicked(it->getStart(),it->getEnd());
    }
}

void MatchesTable::slotShowAll()
{
    showOrHideAll(false);
}

void MatchesTable::slotHideAll()
{
    showOrHideAll(true);
}

