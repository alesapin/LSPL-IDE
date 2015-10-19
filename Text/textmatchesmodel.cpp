#include "textmatchesmodel.h"
#include <QDebug>


TextMatchesModel::TextMatchesModel(QWidget *parent): QAbstractTableModel(parent),currentTab(0)
{
    datum.resize(1);
    currentPatterns.resize(1);
}

int TextMatchesModel::rowCount(const QModelIndex &parent) const
{
    int result  = 0;
    if(currentPatterns.size() > 0) {
        for(const QString& pattern:currentPatterns[currentTab]){
            result+= datum[currentTab][pattern].size();
        }
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
            return current.transform;
         case 2:
            return current.params;
         default:
            return QVariant();
        }
    }
    if(role == Qt::ToolTipRole){
        PatternCompiler::MatchRepr current = getRow(row);
        QString result = "<FONT COLOR=white> <strong> start:</strong> %1 <br> "
                         "<strong> end:</strong> %2 <br> <strong> text: </strong> %3";
        result = result.arg(QString::number(current.start),QString::number(current.end),current.text);
        if(!current.transform.isEmpty()){
            result += "<br> <strong>transform: </strong> %4";
            result = result.arg(current.transform.replace("<","&lt;").replace(">","&gt;"));
        }
        if(!current.params.isEmpty()){
            result += "<br> <strong>params: </strong> %5";
            result = result.arg(current.params);
        }
        result+="</font>";
        return result;
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
                   return QString("Результат преобразования");
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
    qDebug() <<"COUNT:"<< count;
    endRemoveRows();
    return true;


}

bool TextMatchesModel::clearTable()
{

    return removeRows(0,rowCount());
}

void TextMatchesModel::clearDatum()
{
    int oldSize = datum.size();
    datum.clear();
    datum.resize(oldSize);
    currentPatterns.resize(oldSize);
}

void TextMatchesModel::setMatches(const PatternViewMap &maches)
{
    clearTable();
    datum[currentTab].clear();
    currentPatterns[currentTab].clear();
    datum[currentTab] = maches;
    if(currentPatterns[currentTab].isEmpty()){
        currentPatterns[currentTab] << maches.keys();
    }
    int counter = 0;
    for(const QString& currentPattern:currentPatterns[currentTab]){
        insertRows(counter,maches[currentPattern].size());
        counter += maches[currentPattern].size();
    }


}

void TextMatchesModel::setCurrentPatterns(const QStringList &name)
{
    currentPatterns[currentTab] = name;
}

void TextMatchesModel::addCurrentPattern(const QString &name)
{
    int i = 0;
    int row = 0;
    for(;i<currentPatterns[currentTab].size();++i){
            if(currentPatterns[currentTab][i] > name ){
                break;
            }
            row += datum[currentTab][currentPatterns[currentTab][i]].size();
    }
    insertRows(row,datum[currentTab][name].size());
    currentPatterns[currentTab].insert(i,name);
}

void TextMatchesModel::removePatternFromCurrent(const QString &name)
{
    int counter = 0;
    for(const QString& pattern:currentPatterns[currentTab]){
        if(pattern == name){
            break;
        }
        counter += datum[currentTab][pattern].size();
    }
   currentPatterns[currentTab].removeAll(name);
    removeRows(counter,datum[currentTab][name].size());
}

void TextMatchesModel::clearCurrent()
{
    currentPatterns[currentTab].clear();
}

PatternViewMap TextMatchesModel::getCurrentMatches() const
{
    PatternViewMap result;
    for(QString pattern:currentPatterns[currentTab]){
        result [pattern] = datum[currentTab][pattern];
    }
    return result;
}



PatternCompiler::MatchRepr TextMatchesModel::getRow(int index) const
{
    int counter = 0;
    for(const QString& pattern:currentPatterns[currentTab]){
        counter += datum[currentTab][pattern].size();
        if(counter > index){
            counter -= datum[currentTab][pattern].size();
            return datum[currentTab][pattern][index-counter];
        }
    }
}


void TextMatchesModel::changeTab(int index)
{
    qDebug() <<"INDEX:"<< index;
    if(index >= datum.size()){
        clearTable();
        datum.resize(index+1);
        currentPatterns.resize(index+1);
        currentTab = index;
    }else if(index >= 0){
        if(datum[index].isEmpty()){
            clearTable();
            currentTab = index;
        }else if (currentTab != index){
            currentTab = index;
            int counter = 0;
            for(const QString& currentPattern:currentPatterns[currentTab]){
                insertRows(counter,datum[currentTab][currentPattern].size());
                counter += datum[currentTab][currentPattern].size();
            }
        }
    }

}

void TextMatchesModel::closeTab(int index)
{
    qDebug() << "REmove Index: " << index;
    clearTable();
    datum.remove(index);
    currentPatterns.remove(index);
}

QStringList TextMatchesModel::getCurrentPatterns() const
{
    return currentPatterns[currentTab];
}

QStringList TextMatchesModel::getAllPatterns() const
{
    return datum[currentTab].keys();
}

