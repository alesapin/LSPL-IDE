#ifndef TREEITEM_H
#define TREEITEM_H

#include<QList>
#include <QStringList>
#include <QString>
#include <QVariant>
#include "Utils/util.hpp"
enum ColumnNames{
    INTERVAL_COLUMN,TRANSFORM_COLUMN,PARAMS_COLUMN
};

class TreeItem
{
public:
    explicit TreeItem(int s,int l,int pos, TreeItem *m_parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    TreeItem *parentItem();

    void clearChilds();

    int getMatchNumber() const;

    int getStart() const;

    int getEnd() const;

private:
    QList<TreeItem *> childItems;
    int matchNumber;
    int start;
    int end;
    TreeItem *m_parentItem;
};

#endif // TREEITEM_H
