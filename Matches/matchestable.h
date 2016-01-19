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
        return static_cast<MatchesTreeModel*>(model());
    }

public:
    MatchesTable(QWidget *parent = 0);
    void setMatches(QSharedPointer<utility::IntervalViewMap> maches);
    void setCurrentPatterns(const QStringList& name);
    void addCurrentPattern(const QString& name);
    void removePatternFromCurrent(const QString& name);
    void changeTab(int index);
    void closeTab(int index);
    QStringList getCurrentPatterns() const;
    QStringList getAllPatterns() const;
    PatternViewMap getCurrentMatches() const;
    void clear();
protected:
    void resizeEvent(QResizeEvent *event);
signals:
    void rowClicked(int,int);
public slots:
    void onRowClick(const QItemSelection & selected, const QItemSelection & deselected);
};

#endif // MATCHESTABLE_H
