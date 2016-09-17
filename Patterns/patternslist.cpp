#include "patternslist.h"
#include <QDebug>
PatternsList::PatternsList(PatternCompiler* compiler,QWidget *parent):QTableView(parent),compiler(compiler)
{
    myModel = new PatternListModel(this);
    delegate = new PatternItemDelegate(this);
    setModel(myModel);
    setItemDelegate(delegate);
//    setUniformItemSizes(false);
    setSelectionMode(QTableView::SingleSelection);
    setSelectionBehavior(QTableView::SelectRows);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDropIndicatorShown(true);
    setDragDropOverwriteMode(true);
//    setResizeMode(QListView::Adjust);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    removeAction = new QAction("Удалить",this);
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->setColumnWidth(PatternListModel::CHECK_COLUMN,10);
    addAction(removeAction);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    connect(removeAction,SIGNAL(triggered()),this,SLOT(slotRemovePattern()));
    connect(delegate,SIGNAL(textUpdated()),this,SLOT(slotTextEntered()));
}

QStringList PatternsList::getCompiledPatterns() const
{
    return myModel->getCompiledPatterns();
}

QStringList PatternsList::getCompiledPatternsNames() const
{
    return myModel->getCompiledPatternsNames();
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
    int i =0;
    for(const QString& p:patterns){
        addPattern(p);
    }
    resizeRowsToContents();
}

void PatternsList::resizeEvent(QResizeEvent *e)
{
    this->setColumnWidth(PatternListModel::CHECK_COLUMN,30);
    resizeRowsToContents();
    QTableView::resizeEvent(e);
}

void PatternsList::dragMoveEvent(QDragMoveEvent *pEvent)
{
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
    if(!patterns.isEmpty()){
        compiler->clear();
        for(const QString& pattern:patterns){
            QString compilationResult = compiler->compilePatternNoException(pattern);
            myModel->updatePattern(pattern,compilationResult);
        }
    }
    this->viewport()->repaint();
}

void PatternsList::slotRemovePattern()
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();
    for(const QModelIndex& ind: indexes){
        myModel->removeRows(ind.row(),1,ind);
    }
}

void PatternsList::slotTextEntered()
{
    resize(this->width(),this->height()+1);
}

void PatternsList::slotDeleteChecked()
{
    QVector<int> selRows = myModel->getSelectedPatterns();
    for (int i = 0; i < selRows.size(); ++i){
        myModel->removeRows(selRows[i],1,QModelIndex());
    }
}

void PatternsList::slotSelectAll()
{
    myModel->checkAll();
}

void PatternsList::slotDeselectAll()
{
    myModel->resetAll();
}

