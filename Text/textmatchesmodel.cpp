#include "textmatchesmodel.h"
#include <QDebug>


TextMatchesModel::TextMatchesModel(QWidget *parent): QAbstractTableModel(parent)
{

}

int TextMatchesModel::rowCount(const QModelIndex &parent) const
{
    int result  = 0;
    for(const QString& pattern:currentPatterns){
        result+= datum[pattern].size();
    }
    return result;
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
        PatternCompiler::MatchRepr current = getRow(row);
        switch(col){
        case 0:
            return QString("[%1-%2] %3").arg(QString::number(current.start),QString::number(current.end),current.text);
        case 1:
            return current.context;
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
    int end = row + count;
    qDebug() << end;
    beginInsertRows(QModelIndex(), row, end-1);
    //datum.resize(datum.size()+end-row);
    endInsertRows();
    return true;

}

bool TextMatchesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows( QModelIndex(), row, row + count - 1);
    endRemoveRows();
    return true;


}

bool TextMatchesModel::clearTable()
{
    return removeRows(0,datum.size());
}

void TextMatchesModel::setMatches(const PatternViewMap &maches)
{
    datum.clear();
    clearTable();

    for(const QString& patternName:currentPatterns){
        if(maches.find(patternName) == maches.end()){
            currentPatterns.removeAll(patternName);
        }
    }
    datum = maches;
    if(currentPatterns.isEmpty()){
        currentPatterns << maches.keys();
    }
    int counter = 0;
    for(const QString& currentPattern:currentPatterns){
        insertRows(counter,maches[currentPattern].size());
        counter += maches[currentPattern].size();
    }


}

void TextMatchesModel::setCurrentPatterns(const QStringList &name)
{
    currentPatterns = name;
}

void TextMatchesModel::addCurrentPattern(const QString &name)
{
    int i = 0;
    int row = 0;
    for(;i<currentPatterns.size();++i){
            if(currentPatterns[i] > name ){
                break;
            }
            row += datum[currentPatterns[i]].size();
    }
    insertRows(row,datum[name].size());
    currentPatterns.insert(i,name);
}

void TextMatchesModel::removePatternFromCurrent(const QString &name)
{
    int counter = 0;
    for(const QString& pattern:currentPatterns){
        if(pattern == name){
            break;
        }
        counter += datum[pattern].size();
    }
   currentPatterns.removeAll(name);
    removeRows(counter,datum[name].size());
}

void TextMatchesModel::clearCurrent()
{
    currentPatterns.clear();
}



PatternCompiler::MatchRepr TextMatchesModel::getRow(int index) const
{
    int counter = 0;
    for(const QString& pattern:currentPatterns){
        counter += datum[pattern].size();
        if(counter > index){
            counter -= datum[pattern].size();
            return datum[pattern][index-counter];
        }
    }
}

