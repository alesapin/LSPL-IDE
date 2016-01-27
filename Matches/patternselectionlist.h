#ifndef PATTERNSELECTIONLIST_H
#define PATTERNSELECTIONLIST_H
#include <QComboBox>
#include <QWidget>
#include <QStringList>
#include <QStandardItem>
#include <QAbstractItemView>
#include <QStylePainter>
#include "comboselectionmodel.h"
class PatternSelectionList : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QStringList checkedItems READ checkedItems)
public:
    PatternSelectionList(QWidget* parent = 0);
    virtual ~PatternSelectionList();
    QStringList checkedItems() const;
    QStringList unchekedItems() const;
    void checkAll();
    void clearAll();
    void changeTab(int index);
    void closeTab(int index);
    void setUserCheckable(bool);
private:
    ComboSelectionModel* myModel(){
        return models[currentTab];
    }

    int currentTab;
    QStringList labelList;
    QVector<ComboSelectionModel*> models;
    const QRect mDisplayRectDelta;
    void updateDisplayText();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
private slots:
    void slotModelItemChanged(QStandardItem *item);
signals:
    void patternUncheked(const QString& name);
    void patternChecked(const QString& name);
};

#endif // PATTERNSELECTIONLIST_H
