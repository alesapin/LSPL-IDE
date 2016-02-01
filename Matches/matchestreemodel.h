#ifndef MATCHESTREEMODEL_H
#define MATCHESTREEMODEL_H

#include <QVariant>
#include <QModelIndex>
#include <QStringList>
#include <QAbstractItemModel>
#include "treeitem.h"
#include "Utils/util.hpp"
class MatchesTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MatchesTreeModel(QObject *parent = 0);
    ~MatchesTreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    void setMatches(const QSharedPointer<utility::IntervalViewMap> maches);
    void clear();
    QStringList getRowPattern(int rowNum) const;
    QSharedPointer<utility::IntervalViewMap> getMatches() const;
private:
    TreeItem* root;
    QSharedPointer<utility::IntervalViewMap> datum;
    QStringList header;

    QString getToolTipText(int start,int low,int pos) const;
    QVariant getData(TreeItem* it,int column) const;

    const static QString TOOLTIP_BASIC_PATTERN;
    const static QString TOOLTIP_TRANSFORM_PATTERN;
    const static QString TOOLTIP_PARAMS_PATTERN;

};

#endif // MATCHESTREEMODEL_H
