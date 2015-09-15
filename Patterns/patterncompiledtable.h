#ifndef PATTERNCOMPILEDTABLE_H
#define PATTERNCOMPILEDTABLE_H

#include <QAbstractTableModel>
#include <QWidget>
#include <QModelIndex>
#include <QVariant>
#include <QBrush>
#include <QFont>
#include <QVector>
class PatternCompiledTable:public QAbstractTableModel
{
    struct SinglePattern{
        QString patternName;
        int segments;
        int maches;
        int variants;
        bool checked;
    };
    QVector<SinglePattern> datum;
    int findPattern(QString name);
public:
    PatternCompiledTable(QWidget* parent=0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void addPattern(QString patternName,int segments = -1 ,int maches = -1, int variants = -1,bool checked = false);
    void addPatterns(QStringList patterns);
    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
};

#endif // PATTERNCOMPILEDTABLE_H
