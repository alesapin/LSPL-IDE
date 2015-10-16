#include "patterncompiledlist.h"
#include <QDebug>
PatternCompiledList::PatternCompiledList(QWidget *parent):QListWidget(parent)
{
//    model = new QStringListModel(this);

//    setModel(model);
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::SingleSelection);
    editPatternAct = new QAction("Edit Pattern",this);
    editPatternAct->setStatusTip(tr("Make pattern editable"));
    removePatternAct = new QAction("Remove Pattern",this);
    removePatternAct->setStatusTip(tr("Remove pattern from namespace"));
    connect(editPatternAct,SIGNAL(triggered(bool)),this,SLOT(editPattern()));
    connect(removePatternAct,SIGNAL(triggered(bool)),this,SLOT(removePattern()));
    connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this,
            SLOT(patternChanged(QListWidgetItem*)));
    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),
            this,SLOT(showContextMenu(const QPoint &)));
}

void PatternCompiledList::addPattern(const QString &pattern)
{
    QString name = getName(pattern);
    if(!compiledPatterns.contains(pattern)){
        if(!nonCompiledPatterns.contains(name)){
            QListWidgetItem* it =new QListWidgetItem(pattern);
            it->setBackgroundColor(QColor("green"));
            addItem(it);
        }else{
            QListWidgetItem* it = getItem(pattern);
            this->blockSignals(true);
            it->setBackground(QColor("green"));
            it->setFlags(it->flags() ^ Qt::ItemIsEditable);
            this->blockSignals(false);
        }
        compiledPatterns << pattern;
    }
}

void PatternCompiledList::addPatterns(const QStringList &patterns)
{
    for(const QString& pat:patterns){
        addPattern(pat);
    }
//    model->setStringList(compiledPatterns);
}

QStringList PatternCompiledList::getCompiledPatterns() const
{
    QStringList result;
    for(int i = 0; i<compiledPatterns.size();++i){
        QString currentPattern = compiledPatterns[i];
        QStringList spliter = currentPattern.split("=");
        QString name;
        if(spliter.size() >= 1){
            name = spliter[0].simplified();
        }
        result.append(name);
    }
    return result;
}

QStringList PatternCompiledList::getPatternsForCompile() const
{
    QStringList result;
    for(QString name: nonCompiledPatterns.keys()){
        result << name+" = "+nonCompiledPatterns[name];
    }
    return result;
}

void PatternCompiledList::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        emit customContextMenuRequested(event->pos());
    }else{
        QListView::mousePressEvent(event);
    }
}

void PatternCompiledList::showContextMenu(const QPoint &p)
{
        qDebug() << "Request";
        QMenu* current = new QMenu(tr("Context menu"), this);
        current->addAction(editPatternAct);
        current->addAction(removePatternAct);
        current->exec(mapToGlobal(p));
        delete current;
}

void PatternCompiledList::editPattern()
{
    QListWidgetItem* current = currentItem();
    current->setFlags(current->flags () | Qt::ItemIsEditable);
    current->setBackgroundColor(QColor("yellow"));
    compiledPatterns.removeAll(current->text());
    setCurrentItem(current);

}

void PatternCompiledList::removePattern()
{
    QListWidgetItem* current = currentItem();
    compiledPatterns.removeAll(current->text());
    patternChanged(current);
    removeItemWidget(current);
    delete current;
}

void PatternCompiledList::patternChanged(QListWidgetItem *current)
{
    nonCompiledPatterns.clear();
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* it = item(i);
        QString txt = it->text();
        QStringList listRepr = txt.split("=");
        if(!listRepr.isEmpty()){
            QString name = listRepr[0].simplified();
            listRepr.pop_front();
            nonCompiledPatterns[name] = listRepr.join("=").simplified();
        }
    }
}

QString PatternCompiledList::getName(QString pattern)
{
    QStringList listRepr = pattern.split("=");
    if(!listRepr.isEmpty()){
        return listRepr[0].simplified();
    }
    return "";
}

QListWidgetItem *PatternCompiledList::getItem(QString pattern)
{
    for(int i = 0 ;i<count();++i){
        QString itemText = item(i)->text().simplified();
        qDebug() << "Real Name:" << itemText;
        qDebug() << "Patern Name" << pattern;
        if(itemText==pattern){
            return item(i);
        }
    }
    return NULL;
}
