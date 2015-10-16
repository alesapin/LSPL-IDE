#ifndef PATTERNSELECTIONLIST_H
#define PATTERNSELECTIONLIST_H
#include <QComboBox>
#include <QWidget>
#include <QStringList>
#include <QStandardItem>
#include <QAbstractItemView>
#include <QStylePainter>
class PatternSelectionList:public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(QStringList checkedItems READ checkedItems WRITE setCheckedItems)
public:
    PatternSelectionList(QWidget* parent = 0);
    virtual ~PatternSelectionList();
    QStringList checkedItems() const;
    void setCheckedItems(const QStringList &items);
    void clearAll();
private:
    QStringList mCheckedItems;
    QString mDisplayText;
    const QRect mDisplayRectDelta;
    void collectCheckedItems();
    void updateDisplayText();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
private slots:
    void slotModelRowsInserted(const QModelIndex &parent, int start, int end);
    void slotModelRowsRemoved(const QModelIndex &parent, int start, int end);
    void slotModelItemChanged(QStandardItem *item);
signals:
    void patternUncheked(const QString& name);
    void patternChecked(const QString& name);
};

#endif // PATTERNSELECTIONLIST_H
