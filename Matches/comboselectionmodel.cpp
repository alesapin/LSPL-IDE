#include "comboselectionmodel.h"
#include <QDebug>
ComboSelectionModel::ComboSelectionModel(QObject* parent): QStandardItemModel(parent)
{
    userChekable = true;
}

Qt::ItemFlags ComboSelectionModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return Qt::ItemIsEnabled |  Qt::ItemIsUserCheckable;
    }
    if(userChekable){
        return  Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }else{
        return  0;
    }
}

QStringList ComboSelectionModel::getCheckedItems() const
{
    QStringList result;
    for(int i = 0;i<rowCount();++i){
        if(item(i)->checkState() == Qt::Checked){
            result.append(item(i)->text());
        }
    }
    return result;
}

QStringList ComboSelectionModel::getUnchekedItems() const
{
    QStringList result;
    for(int i = 0;i<rowCount();++i){
        if(item(i)->checkState() == Qt::Unchecked){
            result.append(item(i)->text());
        }
    }
    return result;
}

void ComboSelectionModel::checkAll()
{
    for(int i = 0;i<rowCount();++i){
        item(i)->setData(Qt::Checked,Qt::CheckStateRole);
    }
}

void ComboSelectionModel::setChekable(bool val)
{
    userChekable = val;
}





