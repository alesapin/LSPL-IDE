#include "centralwidget.h"
#include <QDebug>
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    watcher = new QFutureWatcher<QSharedPointer<PatternViewMap>>();
    compiler = new PatternCompiler();
    txt = new TextBasicWidget(this);
    pattern = new PatternsBasicWidget(compiler,this);
    matches = new MatchesBasicWidget(this);
    QSplitter* vert = new QSplitter();
    QSplitter* horiz = new QSplitter();
    QWidget* containter = new QWidget();
    QHBoxLayout* containterLay = new QHBoxLayout();
    QVBoxLayout* mainLay = new QVBoxLayout();
    containterLay->setContentsMargins(0,0,0,0);
    horiz->addWidget(txt);
    horiz->addWidget(pattern);
    containterLay->addWidget(horiz);
    containter->setLayout(containterLay);
    vert->setOrientation(Qt::Vertical);
    vert->addWidget(containter);
    vert->addWidget(matches);

//    vert->addWidget(progress);
    mainLay->addWidget(vert);
    setLayout(mainLay);
    connect(txt,SIGNAL(buttonClicked()),this,SLOT(slotAnalyze()));
    connect(matches,SIGNAL(patternWasUnchecked(QString)),txt,SLOT(slotPatternUncheked(QString)));
    connect(matches,SIGNAL(patternWasChecked(QString)),txt,SLOT(slotPatternChecked(QString)));
    connect(matches,SIGNAL(showAll()),txt,SLOT(slotHighlightAll()));
    connect(matches,SIGNAL(hideAll()),txt,SLOT(slotDehighlightAll()));
    connect(matches,SIGNAL(rowClicked(int,int)),txt,SLOT(slotSelectFragment(int,int)));
    connect(matches,SIGNAL(patternWasUnchecked(QString)),this,SIGNAL(statusHighlighting()));
    connect(matches,SIGNAL(patternWasChecked(QString)),this,SIGNAL(statusHighlighting()));
    connect(txt,SIGNAL(tabChanged(int)),matches,SLOT(slotChangeTab(int)));

    connect(txt,SIGNAL(tabClosed(int)),matches,SLOT(slotCloseTab(int)));
    connect(txt,SIGNAL(editEnabled()),matches,SLOT(slotClear()));
    connect(txt,SIGNAL(editEnabled()),this,SLOT(slotEdit()));
    connect(txt,SIGNAL(checkingEnabled()),matches,SLOT(slotEnableChecking()));
    connect(watcher,SIGNAL(finished()),this,SLOT(slotDisplay()));

    connect(txt,SIGNAL(checkingEnabled()),this,SIGNAL(statusReady()));

    //connect(watcher,SIGNAL(progressValueChanged(int)),this,SLOT(slotProgress(int)));
}

CentralWidget::~CentralWidget()
{
    delete compiler;
}

TextBasicWidget *CentralWidget::getTextWidget()
{
    return txt;
}

PatternsBasicWidget *CentralWidget::getPatternWidget()
{
    return pattern;
}

MatchesBasicWidget *CentralWidget::getMatchesWidget()
{
    return matches;
}

PatternCompiler *CentralWidget::getPatternCompiler()
{
    return compiler;
}

QStringList CentralWidget::getChoosenPatterns()
{
    return pattern->getChoosenPatterns();
}

void CentralWidget::slotAnalyze()
{
    QStringList patternNames = pattern->getChoosenPatterns();
    if(!patternNames.isEmpty()){
        emit statusEngine();
        txt->setReadOnly(true);
        QFuture<QSharedPointer<PatternViewMap>> future = QtConcurrent::run(compiler,&PatternCompiler::analyzeText,patternNames,txt->getText());
        watcher->setFuture(future);
    }
}

void CentralWidget::slotDisplay()
{
    if(!watcher->future().isCanceled()){
        QStringList patternNames = pattern->getChoosenPatterns();
        QSharedPointer<PatternViewMap> m = watcher->future().result();
        QSharedPointer<utility::IntervalViewMap> convertedResult = utility::convertMatchesToIntervals(*m);
        if(!convertedResult->empty() && !txt->getText().isEmpty()){
             emit statusHighlighting();
             txt->setMatches(convertedResult);
             matches->setMatches(convertedResult,patternNames);
        }
    }
}

void CentralWidget::slotEdit()
{
    watcher->cancel();
    emit statusCanceled();
}

void CentralWidget::slotProgress(int val)
{
    //qDebug() << "Progress Value:" << val;
}

