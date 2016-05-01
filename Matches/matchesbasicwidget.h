#ifndef MATCHESBASICWIDGET_H
#define MATCHESBASICWIDGET_H
#include "basicwidget.h"
#include "patternselectionlist.h"
#include "matchestable.h"
#include "Engine/patterncompiler.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QDomDocument>
#include <QFile>
#include <QLabel>
#include <QTextStream>
#include <QApplication>
#include <QPushButton>
class MatchesBasicWidget : public BasicWidget
{
    Q_OBJECT
public:
    MatchesBasicWidget(QWidget* parent = 0);
    void setMatches(QSharedPointer<utility::IntervalViewMap> patterns, const QStringList &all = QStringList());
    void setCheckedPatterns(const QStringList& patterns);
    void setCheckedAllPatterns();
    QSharedPointer<utility::IntervalViewMap> getSelectedMatches() const;
    QDomDocument getXml() const;

    void saveMatches(QString filename);

private:
    QLabel* selectPattern;
    PatternSelectionList* list;
    MatchesTable* table;
    QPushButton *selectAll;
    QPushButton *deselectAll;
 signals:
    void rowClicked(int,int);
    void patternWasUnchecked(const QString& name);
    void patternWasChecked(const QString& name);
    void hideAll();
    void showAll();
 public slots:
    void slotClear();
    void slotPatternUnchecked(const QString& s);
    void slotPatternChecked(const QString& s);
    void slotCloseTab(int);
    void slotChangeTab(int);
    void slotEnableChecking();
    void slotSelectAllClicked();
    void slotDeselectAllClicked();

};

#endif // MATCHESBASICWIDGET_H
