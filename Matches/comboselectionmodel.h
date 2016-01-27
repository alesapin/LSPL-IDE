#ifndef COMBOSELECTIONMODEL_H
#define COMBOSELECTIONMODEL_H

#include <QStandardItemModel>
#include <QSet>
class ComboSelectionModel : public QStandardItemModel
{
    Q_OBJECT
public:
    ComboSelectionModel(QObject* parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QStringList getCheckedItems() const;
    QStringList getUnchekedItems() const;
    void checkAll();
    void setChekable(bool);
private:
    bool userChekable;
};

#endif // COMBOSELECTIONMODEL_H
