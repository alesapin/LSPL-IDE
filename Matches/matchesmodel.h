#ifndef MATCHESMODEL_H
#define MATCHESMODEL_H
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QBrush>
#include <QFont>
#include <QVector>
#include "Engine/patterncompiler.h"
/**
 * @brief The MatchesModel class
 * Устаревший неиспользуемый класс
 * Возможно понадобиться в будущем
 */
class MatchesModel:public QAbstractTableModel
{
    Q_OBJECT
private:
    QVector<PatternViewMap> datum;
    QVector<QStringList> currentPatterns;
    int currentTab;
    QString getToolTipText();
public:
    MatchesModel(QWidget* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
    bool clearTable();
    void clearDatum();
    void setMatches(const PatternViewMap& maches);
    void setCurrentPatterns(const QStringList& name);
    void addCurrentPattern(const QString& name);
    void removePatternFromCurrent(const QString& name);
    void clearCurrent();
    PatternViewMap getCurrentMatches() const;
    void changeTab(int index);
    void closeTab(int index);
    QStringList getCurrentPatterns() const;
    QStringList getAllPatterns() const;
    PatternCompiler::MatchRepr getRow(int index) const;
    static const int MATCHES_COLUMNS = 3;
};

#endif // MATCHESMODEL_H
