#include "maintextviewer.h"
#include <QDebug>
const QVector<QPair<int, int> > MainTextViewer::getIntervalsForPattern(const QString &patternName)
{
    QVector<PatternCompiler::MatchRepr> pattern = matches[patternName];
    QVector<QPair<int,int>> result(pattern.size());
    for(int i = 0 ; i < pattern.size();++i){
        result[i] = QPair<int,int>(pattern[i].start,pattern[i].end);
    }
    return result;
}

MainTextViewer::MainTextViewer(QWidget *parent) : QPlainTextEdit(parent),modified(false)
{
    //installEventFilter(this);
    fmtSelect = new QTextCharFormat();
    fmtDeSelect = new QTextCharFormat();
    cursor = new QTextCursor(this->document());

    fmtSelect->setBackground(Qt::yellow);
    converter = QTextCodec::codecForLocale();
    setMinimumHeight(300);
//    setMinimumWidth(300);
    //setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
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

void MainTextViewer::highlightFragments(const QVector<QPair<int, int> > &frags)
{
    for(QPair<int,int> par:frags){
        highlightFragment(par.first,par.second);
    }
}

void MainTextViewer::deHighlightFragment(int begin, int end)
{
    cursor->setPosition(begin,QTextCursor::MoveAnchor);
    cursor->setPosition(end,QTextCursor::KeepAnchor);
    cursor->setCharFormat(*fmtDeSelect);
}

void MainTextViewer::deHighlightFragments(const QVector<QPair<int, int> > &frags)
{
    for(QPair<int,int> par:frags){
        deHighlightFragment(par.first,par.second);
    }
}

void MainTextViewer::setMatches(const PatternViewMap &m)
{
    dehighlightPatterns(matches.keys());
    matches = m;
    highlightPatterns(matches.keys());
}

void MainTextViewer::highlightPatterns(const QStringList &patternNames)
{
    for(const QString& pattern:patternNames){
        highlightFragments(getIntervalsForPattern(pattern));
    }
}

void MainTextViewer::dehighlightPatterns(const QStringList &patternNames)
{
    for(const QString& pattern:patternNames){
        deHighlightFragments(getIntervalsForPattern(pattern));
    }
}

void MainTextViewer::clearSelection()
{
    dehighlightPatterns(matches.keys());
    matches.clear();
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
            if (!cursor.selectedText().isEmpty()){
                QToolTip::showText(helpEvent->globalPos(), cursor.selectedText());
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

