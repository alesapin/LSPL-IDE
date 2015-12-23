#include "patternitemdelegate.h"
#include <QDebug>
PatternItemDelegate::PatternItemDelegate(QWidget *parent)
{
    QFont font;
    font.setFamily(font.defaultFamily());
    metrics =new QFontMetrics(font);

}

void PatternItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    if (index.data().canConvert<PatternListModel::ListItem>()) {
            PatternListModel::ListItem current = qvariant_cast<PatternListModel::ListItem>(index.data());
            if(current.state == PatternListModel::UnCompiled){
                painter->setBrush(QBrush(QColor(255,255,0,127)));
            }else if (current.state ==PatternListModel::Compiled){
                painter->setBrush(QBrush(QColor(0,255,0,127)));
            }else if (current.state == PatternListModel::FailCompiled){
                painter->setBrush(QColor(255,0,0,127));
            }
            painter->drawRect(option.rect);
            painter->drawText(option.rect,Qt::AlignLeft | Qt::TextWordWrap,current.name+" = "+current.text);
        }
}

QSize PatternItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<PatternListModel::ListItem>()) {
        PatternListModel::ListItem current = qvariant_cast<PatternListModel::ListItem>(index.data());
        QRect b = metrics->boundingRect(option.rect,Qt::TextWordWrap,current.name+" = "+current.text);

        return QSize(b.width(),b.height());
    }
    return QStyledItemDelegate::sizeHint(option,index);
}

