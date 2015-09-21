#ifndef TEXTMATCHESTABLE_H
#define TEXTMATCHESTABLE_H

#include <QWidget>
#include <QTableView>
#include <Text/textmatchesmodel.h>
#include <QHeaderView>
#include <QItemSelection>
class TextMatchesTable : public QTableView
{
    Q_OBJECT
public:
    explicit TextMatchesTable(QWidget *parent = 0);
    void setMatches(const PatternViewMap& maches);
    void setCurrentPatterns(const QStringList& name);
    void addCurrentPattern(const QString& name);
    void removePatternFromCurrent(const QString& name);
    void changeTab(int index);
    QStringList getCurrentPatterns() const;
    QStringList getAllPatterns() const;

signals:
    void rowClicked(int,int);
public slots:
    void onRowClick(const QItemSelection & selected, const QItemSelection & deselected);
private:
    TextMatchesModel* model;

};

#endif // TEXTMATCHESTABLE_H
