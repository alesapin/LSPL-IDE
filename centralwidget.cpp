#include "centralwidget.h"
#include <QDebug>
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
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
    mainLay->addWidget(vert);
    setLayout(mainLay);
    connect(txt,SIGNAL(buttonClicked()),this,SLOT(analyze()));
    connect(matches,SIGNAL(patternWasUnchecked(QString)),txt,SLOT(slotPatternUncheked(QString)));
    connect(matches,SIGNAL(patternWasChecked(QString)),txt,SLOT(slotPatternChecked(QString)));
    connect(matches,SIGNAL(rowClicked(int,int)),txt,SLOT(slotSelectFragment(int,int)));
    connect(txt,SIGNAL(tabChanged(int)),matches,SLOT(slotChangeTab(int)));
    connect(txt,SIGNAL(tabClosed(int)),matches,SLOT(slotCloseTab(int)));
    connect(txt,SIGNAL(editEnabled()),matches,SLOT(slotClear()));
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

QStringList CentralWidget::getChoosenPatterns()
{
    return pattern->getChoosenPatterns();
}

void CentralWidget::analyze()
{
    QStringList patternNames = pattern->getChoosenPatterns();
    PatternViewMap result =  compiler->analyzeText(patternNames,txt->getText());
    QSharedPointer<utility::IntervalViewMap> convertedResult = utility::convertMatchesToIntervals(result);
    if(!result.empty() && !txt->getText().isEmpty()){
         txt->setMatches(convertedResult);
         matches->setMatches(convertedResult, patternNames);
     }
}

