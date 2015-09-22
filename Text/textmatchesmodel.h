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
    QVector<PatternViewMap> datum;
    QVector<QStringList> currentPatterns;
    int currentTab;
public:
    TextMatchesModel(QWidget* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
    bool clearTable();
    void setMatches(const PatternViewMap& maches);
    void setCurrentPatterns(const QStringList& name);
    void addCurrentPattern(const QString& name);
    void removePatternFromCurrent(const QString& name);
    void clearCurrent();
    PatternCompiler::MatchRepr getRow(int index) const;
    void changeTab(int index);
    void closeTab(int index);
    QStringList getCurrentPatterns() const;
    QStringList getAllPatterns() const;
signals:

public slots:
};

#endif // TEXTMATCHESMODEL_H
