#include "patternitemdelegate.h"
#include <QDebug>
PatternItemDelegate::PatternItemDelegate(QWidget *parent): QStyledItemDelegate(parent)
{
    QFont font;
    font.setFamily(font.defaultFamily());
    metrics = new QFontMetrics(font);
}

void PatternItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QStyleOptionViewItemV4 opt = option;
    QStyledItemDelegate::initStyleOption(&opt, index);

    if(index.column() == PatternListModel::DATA_COLUMN) {
        if (index.data().canConvert<PatternListModel::ListItem>()) {
                PatternListModel::ListItem current = qvariant_cast<PatternListModel::ListItem>(index.data());
                if(current.state == PatternListModel::UnCompiled){
                    painter->setBrush(QBrush(QColor(255,255,0,127)));
                }else if (current.state ==PatternListModel::Compiled){
                    painter->setBrush(QBrush(QColor(0,255,0,127)));
                }else if (current.state == PatternListModel::FailCompiled){
                    painter->setBrush(QColor(255,0,0,127));
                }
                painter->drawRect(opt.rect);
                opt.rect.setLeft(opt.rect.left()+3);
                painter->drawText(opt.rect,Qt::AlignLeft | Qt::TextWordWrap,current.name+" = "+current.text);
            }
    }
    painter->restore();
}

QSize PatternItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<PatternListModel::ListItem>()) {
        PatternListModel::ListItem current = qvariant_cast<PatternListModel::ListItem>(index.data());
        QRect b = option.fontMetrics.boundingRect(option.rect,Qt::TextWordWrap,current.name+" = "+current.text);
        return QSize(b.width(),b.height());
    }
    return QStyledItemDelegate::sizeHint(option,index);
}

void PatternItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    DelegateTextEdit * e = qobject_cast< DelegateTextEdit* > ( editor );
    QString text = e->toPlainText();
    if(!text.isEmpty() && text.contains("=")) {
        model->setData( index, e->toPlainText() );
        emit textUpdated();
     }
}

void PatternItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    DelegateTextEdit* e = qobject_cast<DelegateTextEdit*>(editor);
    PatternListModel::ListItem current = qvariant_cast<PatternListModel::ListItem>(index.data());
    e->setPlainText(current.name + " = " + current.text);
}

QWidget *PatternItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED( option );
    Q_UNUSED( index );
    if(index.column() == PatternListModel::DATA_COLUMN){
        DelegateTextEdit *result = new DelegateTextEdit(parent);
        connect( result, SIGNAL( editingFinished() ),
            this, SLOT( slotCommitAndClose() ) );
        return result;
    }
    return nullptr;
}

void PatternItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED( index );
    editor->setGeometry( option.rect );
}

void PatternItemDelegate::slotCommitAndClose()
{
    DelegateTextEdit * editor = qobject_cast< DelegateTextEdit* > ( sender() );
    emit commitData( editor );
    emit closeEditor( editor );
}

