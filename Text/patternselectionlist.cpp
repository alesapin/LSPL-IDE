#include "patternselectionlist.h"
#include <QDebug>




PatternSelectionList::PatternSelectionList(QWidget *parent):QComboBox(parent), mDisplayRectDelta(4, 1, -25, 0)
{
    connect(model(), SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(slotModelRowsInserted(QModelIndex,int,int)));
    connect(model(), SIGNAL(rowsRemoved(QModelIndex, int, int)), this, SLOT(slotModelRowsRemoved(QModelIndex,int,int)));
    QStandardItemModel *standartModel = static_cast<QStandardItemModel*>(model());
    connect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
    setStyleSheet("QComboBox { combobox-popup: 1px }");

}

PatternSelectionList::~PatternSelectionList()
{

}

QStringList PatternSelectionList::checkedItems() const
{
    return mCheckedItems;
}

void PatternSelectionList::setCheckedItems(const QStringList &items)
{
    QStandardItemModel *standartModel = qobject_cast<QStandardItemModel*>(model());
    disconnect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));

    for (int i = 0; i < items.count(); ++i)
    {
        int index = findText(items.at(i));
        if (index != -1)
        {
            standartModel->item(index)->setData(Qt::Checked, Qt::CheckStateRole);
        }
    }

    connect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
    collectCheckedItems();
    updateDisplayText();
    repaint();
}

void PatternSelectionList::clearAll()
{
    mCheckedItems.clear();
    mDisplayText.clear();
    clear();
}

void PatternSelectionList::collectCheckedItems()
{
    QStandardItemModel *standartModel = static_cast<QStandardItemModel*>(model());
    mCheckedItems.clear();
    for (int i = 0; i < count(); ++i)
    {
            QStandardItem *currentItem = standartModel->item(i);
            Qt::CheckState checkState = static_cast<Qt::CheckState>(currentItem->data(Qt::CheckStateRole).toInt());
            if (checkState == Qt::Checked)
            {
                mCheckedItems.push_back(currentItem->text());
            }
    }
}

void PatternSelectionList::updateDisplayText()
{
    QRect textRect = rect().adjusted(mDisplayRectDelta.left(), mDisplayRectDelta.top(),
                                     mDisplayRectDelta.right(), mDisplayRectDelta.bottom());

    QFontMetrics fontMetrics(font());

    mDisplayText = mCheckedItems.join(", ");

    if (fontMetrics.size(Qt::TextSingleLine, mDisplayText).width() > textRect.width())
    {
        while (mDisplayText != "" && fontMetrics.size(Qt::TextSingleLine, mDisplayText + "...").width() > textRect.width())
        {
            mDisplayText.remove(mDisplayText.length() - 1, 1);
        }

        mDisplayText += "...";
    }
    //qDebug() << "DISPLAY TEXT: " << mDisplayText;
}

void PatternSelectionList::paintEvent(QPaintEvent *event)
{
    (void)event;

    QStylePainter painter(this);

    painter.setPen(palette().color(QPalette::Text));

    QStyleOptionComboBox option;

    initStyleOption(&option);

    painter.drawComplexControl(QStyle::CC_ComboBox, option);

    QRect textRect = rect().adjusted(mDisplayRectDelta.left(), mDisplayRectDelta.top(),
                                     mDisplayRectDelta.right(), mDisplayRectDelta.bottom());

    painter.drawText(textRect, Qt::AlignVCenter, mDisplayText);
}

void PatternSelectionList::resizeEvent(QResizeEvent *event)
{
    (void)event;

    updateDisplayText();
}

void PatternSelectionList::slotModelRowsInserted(const QModelIndex &parent, int start, int end)
{
        (void)parent;

        QStandardItemModel *standartModel = static_cast<QStandardItemModel*>(model());
        disconnect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
        for (int i = start; i <= end; ++i)
        {
            standartModel->item(i)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            standartModel->item(i)->setData(Qt::Unchecked, Qt::CheckStateRole);
        }
        connect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
}

void PatternSelectionList::slotModelRowsRemoved(const QModelIndex &parent, int start, int end)
{
    (void)parent;
    (void)start;
    (void)end;

    collectCheckedItems();
}

void PatternSelectionList::slotModelItemChanged(QStandardItem *item)
{
    if(item->checkState()==Qt::Checked){
        emit patternChecked(item->text());
    }else if (item->checkState() == Qt::Unchecked){
        emit patternUncheked(item->text());
    }
    collectCheckedItems();
    setCheckedItems(mCheckedItems);
}
