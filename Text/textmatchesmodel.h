#ifndef TEXTMATCHESMODEL_H
#define TEXTMATCHESMODEL_H

#include <QWidget>
#include <QAbstractTableModel>
#include <Engine/patterncompiler.h>
#include <QModelIndex>
#include <QVariant>
#include <QBrush>
#include <QFont>
#include <QVector>
#include <lspl/text/Text.h>
#include <lspl/text/Match.h>
#define MATCH_COLUMNS 3
class TextMatchesModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QVector<PatternCompiler::MatchRepr> datum;
public:
    TextMatchesModel(QWidget* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
    bool clearTable();
    void setMatches(const QVector<PatternCompiler::MatchRepr>& maches);
signals:

public slots:
};

#endif // TEXTMATCHESMODEL_H
