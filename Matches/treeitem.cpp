#include "treeitem.h"

TreeItem::TreeItem(int s,int e,int pos, TreeItem *parentItem):start(s),end(e)
{
    matchNumber = pos;
    this->m_parentItem = parentItem;

}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *child)
{
    childItems.append(child);
}

TreeItem *TreeItem::child(int row)
{
    return childItems.at(row);
}

int TreeItem::childCount() const
{
    return childItems.size();
}

int TreeItem::columnCount() const
{
    return 3;
}

int TreeItem::row() const
{
    if(m_parentItem){
        return m_parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    }
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

void TreeItem::clearChilds()
{
    childItems.clear();
}
int TreeItem::getMatchNumber() const
{
    return matchNumber;
}
int TreeItem::getStart() const
{
    return start;
}
int TreeItem::getEnd() const
{
    return end;
}





