#include "maintextviewer.h"
#include <QDebug>

QString MainTextViewer::getToolTip(int start, int end) const
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

MainTextViewer::MainTextViewer(QWidget *parent) : QPlainTextEdit(parent),modified(false)
{
    fmtSelect = new QTextCharFormat();
    fmtDeSelect = new QTextCharFormat();
    cursor = new QTextCursor(this->document());
    intervalMatches = QSharedPointer<utility::IntervalViewMap>(new utility::IntervalViewMap());
    fmtSelect->setBackground(Qt::yellow);
    converter = QTextCodec::codecForLocale();
    setMinimumHeight(300);
    connect(this,SIGNAL(textChanged()),this,SLOT(modify()));
}

bool MainTextViewer::isModified()
{
    return modified>1;
}

void MainTextViewer::highlightFragment(int begin, int end)
{
    cursor->setPosition(begin,QTextCursor::MoveAnchor);
    cursor->setPosition(end,QTextCursor::KeepAnchor);
    cursor->setCharFormat(*fmtSelect);

}


void MainTextViewer::deHighlightFragment(int begin, int end)
{
    qDebug() << begin << "---" << end;
    cursor->setPosition(begin,QTextCursor::MoveAnchor);
    cursor->setPosition(end,QTextCursor::KeepAnchor);
    cursor->setCharFormat(*fmtDeSelect);
}

void MainTextViewer::dehighlightAll()
{
    cursor->setPosition(0,QTextCursor::MoveAnchor);
    cursor->setPosition(this->toPlainText().length(),QTextCursor::KeepAnchor);
    cursor->setCharFormat(*fmtDeSelect);
}


void MainTextViewer::setMatches(QSharedPointer<utility::IntervalViewMap> m)
{
    offPatterns.clear();
    dehighlightAll();
    intervalMatches = m;//utility::convertMatchesToIntervals(m);
    highlightAll();

}

void MainTextViewer::highlightPatterns(const QStringList &patternNames)
{
    for(const QString& patrn : patternNames){
        offPatterns.remove(patrn);
    }
    for(utility::IntervalViewMap::iterator it = intervalMatches->begin();it!=intervalMatches->end();++it){
        for(const QString& pattern: patternNames){
            if(it->value.patterns.contains(pattern)){
                highlightFragment(it->low,it->high);
                break;
            }
        }
    }
}

void MainTextViewer::highlightAll()
{
    for(utility::IntervalViewMap::iterator it = intervalMatches->begin();it!=intervalMatches->end();++it){
        highlightFragment(it->low,it->high);
    }
}


void MainTextViewer::dehighlightPattern(const QString &pattern)
{
    int prevHigh=0;
    if(!offPatterns.contains(pattern)){
        offPatterns.insert(pattern);
        for(utility::IntervalViewMap::iterator it = intervalMatches->begin();it!=intervalMatches->end();++it){
            bool turnOff = true;
            for(const QString& inner: it->value.patterns){
                if(!offPatterns.contains(inner)){
                    turnOff = false;
                    break;
                }
            }
            if(turnOff){
                deHighlightFragment(it->low > prevHigh ? it->low:prevHigh, it->high);
            }else{
                prevHigh = it->high;
                highlightFragment(it->low,it->high);
            }
        }
    }
}


void MainTextViewer::selectText(int begin, int end)
{
    QTextCursor crs = this->textCursor();
    crs.setPosition(begin);
    crs.setPosition(end,QTextCursor::KeepAnchor);
    this->setTextCursor(crs);

}

bool MainTextViewer::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip)
        {
            QHelpEvent* helpEvent = static_cast<QHelpEvent*>(e);
            QTextCursor cursor = cursorForPosition(helpEvent->pos());
            cursor.select(QTextCursor::WordUnderCursor);
            if (!cursor.selectedText().isEmpty()) {
                QToolTip::showText(helpEvent->globalPos(), getToolTip(cursor.selectionStart(),cursor.selectionEnd()));
            }else{
                QToolTip::hideText();
            }
            return true;
        }
        return QPlainTextEdit::event(e);
}



void MainTextViewer::modify()
{
    modified ++;
}



//void MainTextViewer::insertFromMimeData(const QMimeData *source)
//{
//    //qDebug() << source->data();
//}

