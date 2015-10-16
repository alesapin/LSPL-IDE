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
    setWordWrap(true);
    setTextElideMode(Qt::ElideNone);
    metric = new QFontMetrics(font());
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizeAdjustPolicy(QListWidget::AdjustToContents);
    hide();

}

void PatternCompiledList::addPattern(const QString &pattern)
{
    if(isHidden()) show();
    QString name = getName(pattern);
    if(!compiledPatterns.contains(pattern)){

        if(!nonCompiledPatterns.contains(name)){
            QListWidgetItem* it =new QListWidgetItem(pattern);
            it->setBackgroundColor(QColor(0,255,0,127));
            addItem(it);
        }else{
            QListWidgetItem* it = getItem(pattern);
            this->blockSignals(true);
            it->setBackground(QColor("green"));
            it->setFlags(it->flags() & ~Qt::ItemIsEditable);
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
//    int rows = count();
//    int rowSize = sizeHintForRow(0)+1;
//    qDebug() << "Row Size:" << rowSize;
//    qDebug() << "Rows:"<<rows;
//    int height = rows * rowSize;
//    setFixedHeight(height);
    shrinkToFit();
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

void PatternCompiledList::clearAll()
{
    hide();
    compiledPatterns.clear();
    nonCompiledPatterns.clear();
    clear();
    shrinkToFit();
}

QString PatternCompiledList::getText() const
{
    QString result;
    for(int i = 0;i<count();++i){
        result+=item(i)->text()+'\n';
    }
    if(!result.isEmpty())
        return result.remove(result.size()-1);
    else
        return "";
}

void PatternCompiledList::setUncompiledPatterns(const QStringList& patterns)
{
    for(const QString& pat:patterns){
        if(!pat.isEmpty()){
            addUncompiledPattern(pat);
            QListWidgetItem* it =new QListWidgetItem(pat);
            it->setBackgroundColor(QColor(255,255,0,127));
            addItem(it);
        }
    }
    if(isHidden()) show();
    shrinkToFit();
}

//QSize PatternCompiledList::sizeHint()
//{
//    QSize s;
//    //s.setHeight(QListWidget::sizeHint().height());
//}

//void PatternCompiledList::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button() == Qt::RightButton){
//        emit customContextMenuRequested(event->pos());
//    }else{
//        QListView::mousePressEvent(event);
//    }
//}

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
    current->setBackgroundColor(QColor(255,255,0,127));
    compiledPatterns.removeAll(current->text());
    setCurrentItem(current);

}

void PatternCompiledList::removePattern()
{
    qDebug() <<"Было:" <<count();
    QListWidgetItem* current = currentItem();
    compiledPatterns.removeAll(current->text());
    patternChanged(current);
    removeItemWidget(current);
    delete current;
    if(count() == 0){
        hide();
    }
    shrinkToFit();
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

void PatternCompiledList::addUncompiledPattern(const QString &name)
{
    QStringList listRepr = name.split("=");
    if(!listRepr.isEmpty()){
        QString name = listRepr[0].simplified();
        listRepr.pop_front();
        nonCompiledPatterns[name] = listRepr.join("=").simplified();
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

int PatternCompiledList::getItemHeight(QListWidgetItem *it)
{
    QString text = it->text();
    int w = metric->width(text);
    int numberOfLines =w/width()+1;
    qDebug() << numberOfLines;
    if(numberOfLines > 1){
        return numberOfLines*(metric->height()+1);
    }else{
        return numberOfLines*(metric->height()+2);

    }
}

void PatternCompiledList::shrinkToFit()
{
    int height = 0;
    for(int i = 0;i<count();++i){
        height+= getItemHeight(item(i));
    }
    setFixedHeight(height);
}



QListWidgetItem *PatternCompiledList::getItem(QString pattern)
{
    for(int i = 0 ;i<count();++i){
        QString itemText = item(i)->text().simplified();
        if(itemText==pattern){
            return item(i);
        }
    }
    return NULL;
}
