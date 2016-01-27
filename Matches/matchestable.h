#ifndef MATCHESTABLE_H
#define MATCHESTABLE_H

#include <QTableView>
#include "Engine/patterncompiler.h"
#include "matchesmodel.h"
#include <QHeaderView>
#include <QResizeEvent>
#include <QTreeView>
#include "matchestreemodel.h"
class MatchesTable : public QTreeView
{
    Q_OBJECT
private:
    MatchesTreeModel* myModel() const{
        return models[currentTab];
    }
    int currentTab;
    QVector<MatchesTreeModel*> models;
public:
    MatchesTable(QWidget *parent = 0);
    void setMatches(QSharedPointer<utility::IntervalViewMap> maches);
    void patternVisibility(const QString& patternName,bool hide);
    void hidePattern(const QString& patternName);
    void showPattern(const QString& patternName);
    void changeTab(int index);
    void closeTab(int index);

    QSharedPointer<utility::IntervalViewMap> getCurrentMatches() const;
    void clear();
protected:
    void resizeEvent(QResizeEvent *event);
signals:
    void rowClicked(int,int);
public slots:
    void slotOnRowClick(const QItemSelection & selected, const QItemSelection & deselected);
};

#endif // MATCHESTABLE_H
