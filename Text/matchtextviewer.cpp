#include "matchtextviewer.h"
#include <QDebug>

QString MatchTextViewer::getToolTip(int start, int end) const
{
    QVector<utility::IntervalMatch> mInterval = intervalMatches->getAllIntersections(start,end);
    QString result = "";

    for(int i = 0 ;i<mInterval.size();++i){
        for(int j =0;j<mInterval[i].patterns.size();++j){
            if(!offPatterns.contains(mInterval[i].patterns[j])){
                result +=   mInterval[i].patterns[j];
                result += " - " + mInterval[i].text + '\n';
            }
        }
    }
    return result;
}

void MatchTextViewer::parallelHighlightPatterns()
{
    int i = 0;
    for(utility::IntervalViewMap::iterator it = intervalMatches->begin();it!=intervalMatches->end();++it){
        for(const QString& pattern: highlightedPatterns){
            if(it->value.patterns.contains(pattern)){
                emit highlightIt(it->low,it->high,true);
                i++;
                if(i%100==0){
                    this->thread()->wait(100);
                    watcher->pause();
                }
                break;
            }
            if(watcher->isCanceled()){
                dehighlightAll();
                return;
            }
        }
    }
    emit highlightIt(-1,-1,true);
}

void MatchTextViewer::parallelDehighlightPattern()
{
    int prevHigh=0;
    int i = 0;
    for(utility::IntervalViewMap::iterator it = intervalMatches->begin();it!=intervalMatches->end();++it){
        bool turnOff = true;
        for(const QString& inner: it->value.patterns){
            if(!offPatterns.contains(inner)){
                turnOff = false;
                break;  
            }
        }
        if(turnOff){
            emit highlightIt(it->low > prevHigh ? it->low:prevHigh, it->high, false);
            i++;
        }else{
            prevHigh = it->high;
            emit highlightIt(it->low,it->high,true);
            i++;
        }
        if(watcher->isCanceled()){
            dehighlightAll();
            return;
        }
        if(i%100==0){
            this->thread()->wait(100);
            watcher->pause();
        }
    }
    emit highlightIt(-1,-1,true);
}

void MatchTextViewer::parallelHighlightAll()
{
    int i = 0;
    for(utility::IntervalViewMap::iterator it = intervalMatches->begin();it!=intervalMatches->end();++it){
        emit highlightIt(it->low,it->high,true);
        i++;
        if(watcher->isCanceled()){
            dehighlightAll();
            return;
        }
        if(i%100==0){
            this->thread()->wait(100);
            watcher->pause();
        }
    }
    emit highlightIt(-1,-1,true);
    //qDebug() << "FINISHED";
}

MatchTextViewer::MatchTextViewer(QWidget *parent) : QPlainTextEdit(parent),modified(false)
{
    selectionCursor = new QTextCursor(this->document());
    intervalMatches = QSharedPointer<utility::IntervalViewMap>(new utility::IntervalViewMap());
    watcher = new QFutureWatcher<void>();
    setMinimumHeight(300);
    connect(this,SIGNAL(textChanged()),this,SLOT(slotModify()));
    connect(this,SIGNAL(highlightIt(int,int,bool)),this,SLOT(slotHighlighFragment(int,int,bool)));
    connect(watcher,SIGNAL(finished()),this,SIGNAL(jobDone()));
}

bool MatchTextViewer::isModified()
{
    return modified>1;
}


void MatchTextViewer::dehighlightAll()
{
    setExtraSelections(QList<QTextEdit::ExtraSelection>());
    emit jobDone();
}


void MatchTextViewer::setMatches(QSharedPointer<utility::IntervalViewMap> m)
{
    offPatterns.clear();
    dehighlightAll();
    intervalMatches = m;
    highlightAll();
}

QSharedPointer<utility::IntervalViewMap> MatchTextViewer::getMatches() const
{
    return intervalMatches;
}

void MatchTextViewer::highlightPatterns(const QStringList &patternNames)
{
    for(const QString& patrn : patternNames){
        offPatterns.remove(patrn);
    }
    highlightedPatterns += patternNames.toSet();
    if(!watcher->isRunning()){
        slotHighlightPatterns();
    }else{
        connect(watcher,SIGNAL(finished()),this,SLOT(slotHighlightPatterns()));
    }
}

void MatchTextViewer::highlightAll()
{
    if(!watcher->isRunning()){
        slotHighlightAll();
    }else{
        connect(watcher,SIGNAL(finished()),this,SLOT(slotHighlightAll()));
    }
}


void MatchTextViewer::dehighlightPattern(const QString &pattern)
{
    highlightedPatterns.remove(pattern);
    if(!offPatterns.contains(pattern)){
        offPatterns.insert(pattern);
        if(!watcher->isRunning()){
            slotDehighlightPattern();
        }else{
            connect(watcher,SIGNAL(finished()),this,SLOT(slotDehighlightPattern()));
        }
    }
}


void MatchTextViewer::selectText(int begin, int end)
{
    QTextCursor crs = this->textCursor();
    crs.setPosition(begin);
    crs.setPosition(end,QTextCursor::KeepAnchor);
    this->setTextCursor(crs);

}

void MatchTextViewer::stopCalcing()
{
    watcher->cancel();
}

bool MatchTextViewer::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip){
            QHelpEvent* helpEvent = static_cast<QHelpEvent*>(e);
            QTextCursor cursor = cursorForPosition(helpEvent->pos());
            cursor.select(QTextCursor::WordUnderCursor);
            if (!cursor.selectedText().isEmpty() && isReadOnly()) {
                QToolTip::showText(helpEvent->globalPos(), getToolTip(cursor.selectionStart(),cursor.selectionEnd()));
            }else{
                QToolTip::hideText();
            }
            return true;
        }
    return QPlainTextEdit::event(e);
}

void MatchTextViewer::slotHighlightPatterns()
{
    disconnect(watcher,SIGNAL(finished()),this,SLOT(slotHighlightPatterns()));
    QFuture<void> f = QtConcurrent::run(this,&MatchTextViewer::parallelHighlightPatterns);
    watcher->setFuture(f);
}

void MatchTextViewer::slotDehighlightPattern()
{
    disconnect(watcher,SIGNAL(finished()),this,SLOT(slotDehighlightPattern()));
    QFuture<void> f = QtConcurrent::run(this,&MatchTextViewer::parallelDehighlightPattern);
    watcher->setFuture(f);
}

void MatchTextViewer::slotHighlightAll()
{
    disconnect(watcher,SIGNAL(finished()),this,SLOT(slotHighlightAll()));
    QFuture<void> f = QtConcurrent::run(this,&MatchTextViewer::parallelHighlightAll);
    watcher->setFuture(f);
}



void MatchTextViewer::slotModify()
{
    modified ++;
}

void MatchTextViewer::slotHighlighFragment(int begin, int end,bool select)
{
    if(end != -1){
        selectionCursor->setPosition(begin,QTextCursor::MoveAnchor);
        selectionCursor->setPosition(end,QTextCursor::KeepAnchor);

        QTextEdit::ExtraSelection current;
        current.cursor = *selectionCursor;
        if(select){
            current.format.setBackground(Qt::yellow);
        }else{
            current.format.setBackground(Qt::transparent);
        }
        tmpSelections.append(current);
        if(tmpSelections.size()%100 == 0){
            setExtraSelections(tmpSelections);
            watcher->resume();
        }
    } else {
        setExtraSelections(tmpSelections);
        tmpSelections.clear();
        watcher->resume();
    }
}



//void MatchTextViewer::insertFromMimeData(const QMimeData *source)
//{
//    ////qDebug() << source->data();
//}

