#ifndef MATCHESWIDGET_H
#define MATCHESWIDGET_H


#include <QWidget>
#include <Text/patternselectionlist.h>
#include <Text/textmatchestable.h>
#include <QVBoxLayout>
#include <Engine/patterncompiler.h>
#include <QLabel>
class MatchesWidget:public QWidget
{
    Q_OBJECT
public:
    MatchesWidget(QWidget* parent = 0);
    void setMatches(const PatternViewMap& patterns);
    void setCheckedPatterns(const QStringList& patterns);
    void setCheckedAllPatterns();
    void changeTab(int);

private:
    QLabel* selectPattern;
    PatternSelectionList* list;
    TextMatchesTable* table;
 signals:
    void rowClicked(int,int);
    void patternWasUnchecked(const QString& name);
    void patternWasChecked(const QString& name);
 public slots:
    void slotTransferSignal(int s,int e);
    void slotPatternUnchecked(const QString& s);
    void slotPatternChecked(const QString& s);
};

#endif // MATCHESWIDGET_H
