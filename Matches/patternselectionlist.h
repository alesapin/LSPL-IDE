#ifndef PATTERNSELECTIONLIST_H
#define PATTERNSELECTIONLIST_H
#include <QComboBox>
#include <QWidget>
#include <QStringList>
#include <QStandardItem>
#include <QAbstractItemView>
#include <QStylePainter>
#include <QStyleFactory>
#include "comboselectionmodel.h"
class PatternSelectionList : public QComboBox
{
    Q_OBJECT
public:
    PatternSelectionList(QWidget* parent = 0);
    virtual ~PatternSelectionList();
    QStringList checkedItems() const;
    QStringList unchekedItems() const;
    void checkAll();
    void clearAll();
    void changeTab(int index);
    void closeTab(int index);
private:
    ComboSelectionModel* myModel(){
        return models[currentTab];
    }
    void updateDisplayText();

    const QRect mDisplayRectDelta;
    int currentTab;
    QStringList labelList;
    QVector<ComboSelectionModel*> models;
protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private slots:
    void slotModelItemChanged(QStandardItem *item);

public slots:
    void slotSetUserCheckable(bool);
    void slotDeselectAll();
    void slotSelectAll();
signals:
    void patternUncheked(const QString& name);
    void patternChecked(const QString& name);

};

#endif // PATTERNSELECTIONLIST_H
