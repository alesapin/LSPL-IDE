#include "patternslist.h"
#include <QDebug>
PatternsList::PatternsList(PatternCompiler* compiler,QWidget *parent):QListView(parent),compiler(compiler)
{
    myModel = new PatternListModel(this);
    setModel(myModel);
    setSizeAdjustPolicy(QListView::AdjustToContents);
    setItemDelegate(new PatternItemDelegate(this));
    setUniformItemSizes(false);
    setSelectionMode(QListView::SingleSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QListView::InternalMove);
    setDropIndicatorShown(true);
    setDragDropOverwriteMode(true);
    setResizeMode(QListView::Adjust);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    removeAction = new QAction("Удалить",this);
    editAction = new QAction("Изменить", this);
    addAction(removeAction);
    addAction(editAction);
    connect(removeAction,SIGNAL(triggered()),this,SLOT(slotRemovePattern()));
    connect(editAction,SIGNAL(triggered()),this,SLOT(slotEditPattern()));

}

QStringList PatternsList::getCompiledPatterns() const
{

    return myModel->getCompiledPatterns();
}

void PatternsList::clearAll()
{
    myModel->clearAll();
}

void PatternsList::addPattern(const QString &text)
{
    myModel->addUncompiledPattern(text);
}

void PatternsList::addPatterns(const QStringList &patterns)
{
    for(const QString& p:patterns){
        addPattern(p);
    }
}

void PatternsList::dragMoveEvent(QDragMoveEvent *pEvent)
{
//    qDebug() << "CALLED DRAG";
//    pEvent->
    QModelIndex index = indexAt(pEvent->pos());

    pEvent->acceptProposedAction();
}

void PatternsList::dragEnterEvent(QDragEnterEvent *pEvent)
{
    pEvent->acceptProposedAction();
}

void PatternsList::dropEvent(QDropEvent *pEvent)
{
    QModelIndex index = indexAt(pEvent->pos());
    int end = index.row();
    const QMimeData* dat = pEvent->mimeData();
    int start = dat->data("row").toInt();
    myModel->swapRows(start,end);
    pEvent->acceptProposedAction();
}




void PatternsList::slotCompilePatterns()
{
    QStringList patterns = myModel->getUncompiledPatterns();
    for(const QString& pattern:patterns){
        QString compilationResult = compiler->compilePatternNoException(pattern);
        myModel->updatePattern(pattern,compilationResult);
    }
}

void PatternsList::slotRemovePattern()
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();
    for(const QModelIndex& ind: indexes){
        myModel->removeRows(ind.row(),1,ind);
    }
}

void PatternsList::slotEditPattern()
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();
    if(!indexes.isEmpty()) {
        QModelIndex ind = indexes[0];
        PatternListModel::ListItem dat = qvariant_cast<PatternListModel::ListItem>(ind.data(Qt::DisplayRole));
        emit editPatternSignal(dat.name+" = "+dat.text);
        myModel->removeRows(ind.row(),1,ind);
    }
}

