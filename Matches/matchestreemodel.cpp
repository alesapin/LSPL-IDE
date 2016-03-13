#include "matchestreemodel.h"

  const QString MatchesTreeModel::TOOLTIP_BASIC_PATTERN =  "<font color=white> \
                                                      <strong> pattern: </strong> %1 <br> \
                                                      <strong> start:</strong> %2 <br> \
                                                      <strong> end:</strong> %3 <br> \
                                                     <strong> text: </strong> %4\
                                                     %5 %6 </font>";
  const QString MatchesTreeModel::TOOLTIP_TRANSFORM_PATTERN =  "<br><strong>transform:</strong> %1";
  const QString MatchesTreeModel::TOOLTIP_PARAMS_PATTERN =  "<br><strong>params:</strong> %1";


MatchesTreeModel::MatchesTreeModel(QObject *parent): QAbstractItemModel(parent),datum()
{
       header<<"Сопоставление" <<"Преобразование" <<"Параметры";
       root = new TreeItem(0,0,0);
}

MatchesTreeModel::~MatchesTreeModel()
{
    delete root;
}

QVariant MatchesTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    switch (role) {
    case Qt::DisplayRole:
    {
        if(item == root){
            return header[index.column()];
        }
        return getData(item,index.column());
    }
    case Qt::BackgroundRole:
        if (0 == index.row() % 2)
                return QVariant(QColor(240,240,240));
        else
            return  QVariant(QColor(Qt::white));
    case Qt::ToolTipRole:
        return getToolTipText(item->getStart(),item->getEnd(),item->getMatchNumber());
    default:
        return QVariant();
    }
}

Qt::ItemFlags MatchesTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant MatchesTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return header[section];
    return QVariant();
}

QModelIndex MatchesTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
           return QModelIndex();

       TreeItem *parentItem;

       if (!parent.isValid())
           parentItem = root;
       else
           parentItem = static_cast<TreeItem*>(parent.internalPointer());

       TreeItem *childItem = parentItem->child(row);
       if (childItem)
           return createIndex(row, column, childItem);
       else
           return QModelIndex();
}

QModelIndex MatchesTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->getParentItem();

    if (parentItem == root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int MatchesTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = root;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int MatchesTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
          return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
      else
        return root->columnCount();
}


void MatchesTreeModel::setMatches(const QSharedPointer<utility::IntervalViewMap> maches)
{
    int rc = 0;
    removeRows(0,rowCount());
    root->clearChilds();
    datum = maches;
    TreeItem* currentRoot = root;
    beginInsertRows(QModelIndex(),0,maches->size());
    for(utility::IntervalViewMap::iterator it = maches->begin(); it!= maches->end();++it){
        int start = it->low;
        int end = it->high;
        utility::IntervalMatch currentInterval = it->value;
        if(currentInterval.patterns.size() == 1){
            currentRoot->appendChild(new TreeItem(start,end,0,currentRoot));
            rc++;
        }else{
            TreeItem* currentItem = new TreeItem(start,end,-1,currentRoot);
            for(int i = 0;i<currentInterval.patterns.size();++i){
                currentItem->appendChild(new TreeItem(start,end,i,currentItem));
                rc++;
            }
            currentRoot->appendChild(currentItem);
        }
    }
    endInsertRows();
}

void MatchesTreeModel::clear()
{
    root->clearChilds();
}

QStringList MatchesTreeModel::getRowPattern(int rowNum) const
{
    QModelIndex currentIndex = index(rowNum,0);

    TreeItem* currentItem = static_cast<TreeItem*>(currentIndex.internalPointer());
    if(!currentIndex.isValid() || currentItem == 0) return QStringList();
    try {
        utility::IntervalMatch m = datum->getEqualInterval(currentItem->getStart(),currentItem->getEnd());
        if(currentItem->getMatchNumber() == -1){
            return m.patterns.toList();
        } else {
            return QStringList() << m.patterns[currentItem->getMatchNumber()];
        }
    } catch (IntervalNotFoundException e) {
        return QStringList();
    }
}

QSharedPointer<utility::IntervalViewMap> MatchesTreeModel::getMatches() const
{
    return datum;
}

QString MatchesTreeModel::getToolTipText(int start, int end, int pos) const
{
    try{
        utility::IntervalMatch m = datum->getEqualInterval(start,end);
        QString patterns,transform,params;
        if(pos == -1){
            patterns = "[";
            int i;
            for(i = 0; i<m.patterns.size()-1;++i){
                patterns += m.patterns[i] +", ";
            }
            patterns += m.patterns[i]+"]";
        }else{
            patterns = m.patterns[pos];
            if(!m.transforms[pos].isEmpty()){
                transform = TOOLTIP_TRANSFORM_PATTERN.arg(m.transforms[pos]);
            }
            if(!m.params[pos].isEmpty()){
                params = TOOLTIP_PARAMS_PATTERN.arg(m.params[pos]);
            }
        }
        return TOOLTIP_BASIC_PATTERN.arg(patterns,QString::number(start),QString::number(end),m.text,transform,params);
    } catch (IntervalNotFoundException e){
        return "";
    }
}

QVariant MatchesTreeModel::getData(TreeItem *it, int column) const
{
    int start = it->getStart();
    int end = it->getEnd();
    int pos = it->getMatchNumber();
    try {
        utility::IntervalMatch m = datum->getEqualInterval(start,end);

        switch(column){
        case INTERVAL_COLUMN:
            if(m.transforms.size() == 1){
                return QString("[%1] %3").arg(m.patterns[0],m.text);
            }else if(pos == -1){
                return QString("[%1-%2] %3").arg(QString::number(start),QString::number(end),m.text);
            }else{
                return QString("[%1]").arg(m.patterns[pos]);
            }
        case TRANSFORM_COLUMN:
            if(pos == -1) return "";
            return m.transforms[pos];
        case PARAMS_COLUMN:
            if(pos == -1) return "";
            return m.params[pos];
        default:
            return QVariant("");
        }
    } catch (IntervalNotFoundException& e){
        return QVariant("");
    }
}
