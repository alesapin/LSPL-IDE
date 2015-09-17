#include "textmatchesmodel.h"



TextMatchesModel::TextMatchesModel(QWidget *parent): QAbstractTableModel(parent)
{

}

int TextMatchesModel::rowCount(const QModelIndex &parent) const
{
    return datum.size();
}

int TextMatchesModel::columnCount(const QModelIndex &parent) const
{
    return MATCH_COLUMNS;
}

QVariant TextMatchesModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    if(role == Qt::DisplayRole){
        PatternCompiler::MatchRepr current = datum.at(row);
        switch(col){
        case 0:
            return QString("[%1-%2] %3").arg(QString::number(current.start),QString::number(current.end),current.text);
         default:
            return QVariant();
        }
    }
    return QVariant();

}

QVariant TextMatchesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole){
           if (orientation == Qt::Horizontal) {
               switch (section){
               case 0:
                   return QString("Текст сопоставления");
               case 1:
                   return QString("Контекст");
               case 2:
                   return QString("Параметры");
            }
        }
    }
    return QVariant();
}

bool TextMatchesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    int end = datum.size()+count;
    beginInsertRows(QModelIndex(), datum.size(), end-1);
    //datum.resize(datum.size()+end-row);
    endInsertRows();
    return true;

}

bool TextMatchesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows( QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        datum.erase(datum.begin()+row);
    }
    endRemoveRows();

    return true;


}

bool TextMatchesModel::clearTable()
{
    return removeRows(0,datum.size());
}

void TextMatchesModel::setMatches(const QVector<PatternCompiler::MatchRepr> &maches)
{
    datum.clear();
    clearTable();
    insertRows(0,maches.size());
    datum = maches;

}

