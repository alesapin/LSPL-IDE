#ifndef PATTERNITEMDELEGATE_H
#define PATTERNITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "patternlistmodel.h"
#include "patternhighlighter.h"
#include "delegatetextedit.h"
#include <QPainter>
#include <QItemDelegate>
#include <QStyle>
#include <QApplication>
#include <QVariant>
#include <QTextEdit>
class PatternItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:

    QFontMetrics* metrics;
public:
    PatternItemDelegate(QWidget* parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget * editor, QAbstractItemModel * model,
                      const QModelIndex & index ) const Q_DECL_OVERRIDE;
    void setEditorData( QWidget * editor, const QModelIndex & index ) const Q_DECL_OVERRIDE;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void updateEditorGeometry( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const Q_DECL_OVERRIDE;
public slots:
    void slotCommitAndClose();
signals:
    void textUpdated() const;
};

#endif // PATTERNITEMDELEGATE_H
