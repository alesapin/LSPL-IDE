#ifndef PATTERNITEMDELEGATE_H
#define PATTERNITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "patternlistmodel.h"
#include <QPainter>
#include <QVariant>
class PatternItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:
    QFontMetrics* metrics;
public:
    PatternItemDelegate(QWidget* parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PATTERNITEMDELEGATE_H
