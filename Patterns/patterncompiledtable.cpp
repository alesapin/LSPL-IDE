#include "patterncompiledtable.h"
#include <QDebug>




int PatternCompiledTable::findPattern(QString name)
{
    for(int i = 0 ; i <datum.size();++i){
        if(datum[i].patternName == name){
            return i;
        }
    }
    return -1;
}

PatternCompiledTable::PatternCompiledTable(QWidget *parent):QAbstractTableModel(parent)
{
}

int PatternCompiledTable::rowCount(const QModelIndex &parent) const
{
    return datum.size();
}

int PatternCompiledTable::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant PatternCompiledTable::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    if(role == Qt::DisplayRole){
        SinglePattern current = datum.at(row);
        switch (col) {
        case 0:
            return current.patternName;
            break;
       case 1:
            if(current.segments!=-1){
                return current.segments;
            }
            break;
        case 2:
            if(current.maches != -1){
                return current.maches;
            }
            break;
         case 3:
            if(current.variants != -1){
                return current.variants;
            }
            break;
        default:
            break;
        }
        return "?";
    }else if (role == Qt::FontRole){
        if (col == 0)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
    } else if (role == Qt::CheckStateRole){

        if (col == 0) //add a checkbox to cell(1,0)
        {
            return datum.at(row).checked ? Qt::Checked : Qt::Unchecked;
        }
    }
    return QVariant();
}

QVariant PatternCompiledTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
       {
           if (orientation == Qt::Horizontal) {
               switch (section)
               {
               case 0:
                   return QString("Шаблон");
               case 1:
                   return QString("Отрезков");
               case 2:
                   return QString("Сопоставлений");
                case 3:
                   return QString("Вариантов");
               }
           }
       }
       return QVariant();

}

bool PatternCompiledTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    int row = index.row();
    if (role == Qt::CheckStateRole)
    {
        if ((Qt::CheckState)value.toInt() == Qt::Checked)
        {
            datum[row].checked = true;
            return true;
        }
        else
        {
            datum[row].checked = false;
            return true;
        }
    }
    return false;
}

Qt::ItemFlags PatternCompiledTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if( index.column() == 0){
        flags |= Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
    }
    return flags;
}

void PatternCompiledTable::addPattern(QString patternName, int segments, int maches, int variants, bool checked)
{
    int indx = findPattern(patternName);
    if(indx == -1){
        insertRows(datum.size(),1);
        datum[datum.size()-1] = {patternName,segments,maches,variants,checked};
    }else{
        datum[indx] = {patternName,segments,maches,variants,checked};
        emit(dataChanged(index(indx,0),index(indx,4)));
    }
}

void PatternCompiledTable::addPatterns(QStringList patterns)
{
    for(QString pattern:patterns){
        addPattern(pattern);
    }
}

bool PatternCompiledTable::insertRows(int start, int count, const QModelIndex &parent)
{
     int end = datum.size()+count;
     beginInsertRows(QModelIndex(), datum.size(), end-1);
     for (int row=start; row < end; row++) {
         datum.append({"",-1,-1,-1,false});
      }
      endInsertRows();
      return true;
}
