#include "patternlistmodel.h"
#include <QDebug>
PatternListModel::PatternListModel(QObject *parent):QAbstractListModel(parent)
{
    QFont font;
    font.setFamily(font.defaultFamily());
    par = static_cast<QWidget*>(parent);
    metrics = new QFontMetrics(font);
    connect(this,SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),this,SLOT(slotRowMoved(QModelIndex,int,int,QModelIndex,int)));
    //qRegisterMetaTypeStreamOperators<PatternListModel::ListItem>("PatternListModel::ListItem");

}

QVariant PatternListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    ListItem current = rowData[row];
    if(role == Qt::DisplayRole){
        return QVariant::fromValue(current);
    }
    if(role == Qt::SizeHintRole){
        QRect r = metrics->boundingRect(current.name+" = "+current.text);
        return QSize(r.width(),r.height());

    }

//    if(role==Qt::SizeHintRole){
//
//        return QSize(r.width(),r.height());
//    }
    if(role == Qt::ToolTipRole){
        return current.compilationOutput;
    }
    return QVariant();
}

int PatternListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return rowData.size();
}

void PatternListModel::addUncompiledPattern(const QString &pattern)
{
    QPair<QString,QString> nameBody = splitPattern(pattern);
    if(nameBody.second == "") return;
    ListItem currentItem = {nameBody.first,nameBody.second,UnCompiled,""};
    int i;
    if( (i = rowData.indexOf(currentItem)) !=-1 ){
        rowData[i].text = currentItem.text;
        rowData[i].state = UnCompiled;
        insertRow(i);
    } else {
        rowData.append(currentItem);
        insertRows(rowData.size(),1,QModelIndex());

    }
    recompileAll = true;
}

void PatternListModel::addUncompiledPatterns(const QStringList &patterns)
{
    for(const QString& str:patterns){
        addUncompiledPattern(str);
    }
}

void PatternListModel::updatePattern(const QString &pattern, QString text)
{
    QString patternName = splitPattern(pattern).first;
    ListItem current{patternName,"",UnCompiled,""};
    int index = rowData.indexOf(current);
    if(index!=-1){
        if(!text.isEmpty()){
            rowData[index].state = FailCompiled;
            rowData[index].compilationOutput = text;
        }else{
            rowData[index].state = Compiled;
            rowData[index].compilationOutput = "Compilation Succesfull!";
        }
    }
    emit dataChanged(createIndex(index,0),createIndex(index,0));
    recompileAll = false;
}

QStringList PatternListModel::getUncompiledPatterns() const
{
    /*Либо берем все либо только не скомпилированные*/
    QStringList result;
    if(recompileAll){
        for(int i = 0; i <rowData.size();++i){
            result.append(rowData[i].name+"="+rowData[i].text);
        }
    }else{
        for(int i = 0; i <rowData.size();++i){
            if(rowData[i].state == UnCompiled){
                result.append(rowData[i].name+"="+rowData[i].text);
            }
        }
    }
    return result;

}

QStringList PatternListModel::getCompiledPatterns() const
{
    QStringList result;
    for(int i = 0; i <rowData.size();++i){
        if(rowData[i].state == Compiled){
            result.append(rowData[i].name);
        }
    }
    return result;

}

void PatternListModel::clearAll()
{
    removeRows(0,rowData.size(),QModelIndex());
    rowData.clear();
    recompileAll = false;
}

bool PatternListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    while(count--) rowData.removeAt(row);
    endRemoveRows();
    return true;
}

bool PatternListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for ( int i = row; i < (row + count - 1); i++ )
        rowData.insert(i, ListItem{"","",UnCompiled,""});
    endInsertRows();
}

Qt::ItemFlags PatternListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
     if (index.isValid()){
            return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
      }else{
            return Qt::ItemIsDropEnabled | defaultFlags;
     }
}

QMimeData *PatternListModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* result = new QMimeData();
    result->data(QString::number(indexes.at(0).row()));
    result->setData("row",QByteArray::number(indexes.at(0).row()));
    return result;
}

void PatternListModel::swapRows(int first, int second)
{
    if(first < 0 || first >= rowData.size()) first =rowData.size()-1;
    if(second <0 || second >= rowData.size()) second =rowData.size() - 1;
    if(first < second){
        rowData.insert(second+1,rowData[first]);
        rowData.removeAt(first);
    } else {
        ListItem tmp = rowData[first];
        rowData.removeAt(first);
        rowData.insert(second,tmp);
    }
    setPatternsUncompiled();
    emit dataChanged(createIndex(first,0),createIndex(second,0));
}

QPair<QString, QString> PatternListModel::splitPattern(const QString &pattern)
{
    QStringList listRepr = pattern.split("=");
    if(!listRepr.isEmpty()){
        QString name = listRepr[0].simplified();
        listRepr.pop_front();
        QString body = listRepr.join("=").simplified();
        return qMakePair<QString,QString> (name,body);
    }
    return qMakePair<QString,QString>("","");
}

void PatternListModel::setPatternsUncompiled()
{
    for(int i = 0;i<rowData.size();++i){
        rowData[i].state = UnCompiled;
    }
}

void PatternListModel::slotRowMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
{
    qDebug() << "CALLED";
}

//QDataStream& operator<<(QDataStream& ostream, const PatternListModel::ListItem& ms)
//{
//    ostream << ms.name << ms.text;
//    return ostream;
//}

//QDataStream& operator>>(QDataStream& istream, PatternListModel::ListItem& ms)
//{
//    istream >> ms.name >> ms.text;
//    return istream;
//}
