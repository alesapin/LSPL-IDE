#include "patternlistmodel.h"
#include <QDebug>
PatternListModel::PatternListModel(QObject *parent):QAbstractTableModel(parent)
{
    QFont font;
    font.setFamily(font.defaultFamily());
    par = static_cast<QWidget*>(parent);
    metrics = new QFontMetrics(font);
}

QVariant PatternListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    ListItem current = rowData[row];
    if(col == DATA_COLUMN){
        if(role == Qt::DisplayRole){
            return QVariant::fromValue(current);
        }
        if(role == Qt::SizeHintRole){
            QRect r = metrics->boundingRect(current.name+" = "+current.text);
            return QSize(r.width(),r.height());
        }
        if(role==Qt::EditRole){
            return QVariant::fromValue(current);
        }
        if(role == Qt::ToolTipRole){
            return current.compilationOutput;
        }
    }else{
        if(role == Qt::CheckStateRole){
           return current.checked?Qt::Checked:Qt::Unchecked;
        }
    }
    return QVariant();

}

int PatternListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return rowData.size();
}

bool PatternListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int col = index.column();
    if(col == DATA_COLUMN) {
        if (role == Qt::EditRole){
            QPair<QString,QString> splited = utility::splitPattern(value.toString());
            rowData[index.row()].name = splited.first;
            rowData[index.row()].text = splited.second;
            rowData[index.row()].state = UnCompiled;
            setPatternsUncompiled();
            return true;
        }
    }else{
        if(role == Qt::CheckStateRole){
            rowData[index.row()].checked = !rowData[index.row()].checked;
            return true;
        }
    }
   return QAbstractTableModel::setData(index,value,role);
}

void PatternListModel::addUncompiledPattern(const QString &pattern)
{

    QPair<QString,QString> nameBody = utility::splitPattern(pattern);
    if(nameBody.second == "") return;
    if( positions.contains(nameBody.first)) {
        int pos = positions[nameBody.first];
        rowData[pos].text += " | " + nameBody.second;
        rowData[pos].state = UnCompiled;
        emit dataChanged(createIndex(pos,0),createIndex(pos+1,0));
    } else {
        ListItem currentItem = {nameBody.first,nameBody.second,UnCompiled,"",false};
        positions[nameBody.first] = rowData.size();
        rowData.append(currentItem);
        insertRows(rowData.size(),1,QModelIndex());
    }
    recompileAll = true;
}

void PatternListModel::addUncompiledPatterns(const QStringList &patterns)
{
    int i = 0;
    for(const QString& str:patterns){
        addUncompiledPattern(str);
    }
}

void PatternListModel::updatePattern(const QString &pattern, QString text)
{
    QString patternName = utility::splitPattern(pattern).first;
    ListItem current{patternName,"",UnCompiled,"",false};
    int index = rowData.indexOf(current);
    if(index!=-1){
        if(!text.isEmpty()){
            rowData[index].state = FailCompiled;
            rowData[index].compilationOutput = text;
        }else{
            rowData[index].state = Compiled;
            rowData[index].checked = true;
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
            if(rowData[i].state == UnCompiled || rowData[i].state == FailCompiled){
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
        if(rowData[i].state == Compiled && rowData[i].checked){
            result.append(rowData[i].name + " = " + rowData[i].text);
        }
    }
    return result;

}

QStringList PatternListModel::getCompiledPatternsNames() const
{
    QStringList result;
    for(int i = 0; i <rowData.size();++i){
        if(rowData[i].state == Compiled && rowData[i].checked){
            result.append(rowData[i].name);
        }
    }
    return result;
}

void PatternListModel::clearAll()
{
    removeRows(0,rowData.size(),QModelIndex());
    rowData.clear();
    positions.clear();
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
        rowData.insert(i, ListItem{"","",UnCompiled,"",false});
    endInsertRows();
    return true;
}

Qt::ItemFlags PatternListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
     if (index.isValid()){
            return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled  | Qt::ItemIsSelectable |  Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
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

void PatternListModel::setPatternsUncompiled()
{
    for(int i = 0;i<rowData.size();++i){
        rowData[i].state = UnCompiled;
    }
}
int PatternListModel::columnCount(const QModelIndex &parent) const {
    return 2;
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
