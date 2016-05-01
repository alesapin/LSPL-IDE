#include "patternselectionlist.h"
#include <QDebug>
PatternSelectionList::PatternSelectionList(QWidget *parent):QComboBox(parent), mDisplayRectDelta(4, 1, -25, 0),currentTab(0)
{
    labelList.append("");
    models.append(new ComboSelectionModel());
    setModel(models[currentTab]);
    connect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
#ifndef Q_OS_WIN
    this->setStyle(QStyleFactory::create("Windows"));
#endif
}

PatternSelectionList::~PatternSelectionList()
{

}

QStringList PatternSelectionList::checkedItems() const
{
    return models[currentTab]->getCheckedItems();
}

QStringList PatternSelectionList::unchekedItems() const
{
    return models[currentTab]->getUnchekedItems();
}

void PatternSelectionList::checkAll()
{
    myModel()->checkAll();
    slotSelectAll();
}


void PatternSelectionList::clearAll()
{
    disconnect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
    models.clear();
    labelList.clear();
    models.append(new ComboSelectionModel());
    setModel(models[0]);
    currentTab = 0;
    connect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
    labelList.append("");
    clear();
}

void PatternSelectionList::clearCurrent()
{
    disconnect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
    models[currentTab]->clear();
    labelList[currentTab] = "";
    connect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));

}

void PatternSelectionList::changeTab(int index)
{
    if(currentTab != index){
        disconnect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
        if(index < models.size()) {
            ComboSelectionModel* m = models[index];
            setModel(m);
            currentTab = index;
        }else{
            models.append(new ComboSelectionModel());
            labelList.append("");
            currentTab = models.length() - 1;
            setModel(models[currentTab]);
        }
        connect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
        updateDisplayText();
        repaint();
    }
}

void PatternSelectionList::closeTab(int index)
{
    if(index != -1){
        disconnect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
        int newCurrent = index + 1;
        if(newCurrent < models.length()){
            setModel(models[newCurrent]);
            models.remove(index);
            currentTab = index;
        }else{
             newCurrent = index - 1;
             if(newCurrent >= 0){
                 setModel(models[newCurrent]);
                 models.remove(index);
                 currentTab = newCurrent;
             }else{
                 models[0]->clear();
                 currentTab = 0;
             }
        }
        connect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
        updateDisplayText();
        repaint();
    }
}

void PatternSelectionList::slotSetUserCheckable(bool v)
{
    myModel()->setChekable(v);
}


void PatternSelectionList::updateDisplayText()
{
    QRect textRect = rect().adjusted(mDisplayRectDelta.left(), mDisplayRectDelta.top(),
                                     mDisplayRectDelta.right(), mDisplayRectDelta.bottom());

    QFontMetrics fontMetrics(font());
    labelList[currentTab] = myModel()->getCheckedItems().join(", ");
    labelList[currentTab] = fontMetrics.elidedText(labelList[currentTab],Qt::ElideRight,textRect.width());
}

void PatternSelectionList::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);

    QStylePainter painter(this);

    painter.setPen(palette().color(QPalette::Text));

    QStyleOptionComboBox option;

    initStyleOption(&option);

    painter.drawComplexControl(QStyle::CC_ComboBox, option);

    QRect textRect = rect().adjusted(mDisplayRectDelta.left(), mDisplayRectDelta.top(),
                                     mDisplayRectDelta.right(), mDisplayRectDelta.bottom());

    painter.drawText(textRect, Qt::AlignVCenter, labelList[currentTab]);
}

void PatternSelectionList::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    updateDisplayText();
}


void PatternSelectionList::slotModelItemChanged(QStandardItem *item)
{

    if(item->checkState()==Qt::Checked){
        emit patternChecked(item->text());
    }else if (item->checkState() == Qt::Unchecked){
        emit patternUncheked(item->text());
    }
    updateDisplayText();
    repaint();
    myModel()->setChekable(false);
}

void PatternSelectionList::slotDeselectAll()
{
    disconnect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
    for(int i = 0;i<myModel()->rowCount();++i){
        setItemData(i,Qt::Unchecked,Qt::CheckStateRole);
    }
    updateDisplayText();
    repaint();
    connect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));

}

void PatternSelectionList::slotSelectAll()
{
    disconnect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
    for(int i = 0;i<myModel()->rowCount();++i){
       setItemData(i,Qt::Checked,Qt::CheckStateRole);
    }
    updateDisplayText();
    repaint();
    connect(myModel(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
}


