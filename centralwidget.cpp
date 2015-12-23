#include "centralwidget.h"
#include <QDebug>
CentralWidget::CentralWidget(QWidget *parent) : QMainWindow(parent)
{
    compiler = new PatternCompiler();
    txt = new TextBasicWidget(this);
    pattern = new PatternsBasicWidget(compiler,this);
    matches = new MatchesBasicWidget(this);
    setDockNestingEnabled(true);
    //    connect(txt,SIGNAL(currentChanged(int)),matches,SLOT(changeTab(int)));
    //QDockWidget* dummy = new QDockWidget;
    //addDockWidget(Qt::BottomDockWidgetArea, dummy);
    //removeDockWidget(dummy);
//    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
//    setCorner(Qt::TopRightCorner,Qt::RightDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea,matches);
    addDockWidget(Qt::LeftDockWidgetArea,txt);
    addDockWidget(Qt::RightDockWidgetArea,pattern);
//    tabifyDockWidget(matches,txt);
//    tabifyDockWidget(matches,pattern);

    connect(txt,SIGNAL(buttonClicked()),this,SLOT(analyze()));
    connect(matches,SIGNAL(patternWasUnchecked(QString)),txt,SLOT(slotPatternUncheked(QString)));
    connect(matches,SIGNAL(patternWasChecked(QString)),txt,SLOT(slotPatternChecked(QString)));
    connect(matches,SIGNAL(rowClicked(int,int)),txt,SLOT(slotSelectFragment(int,int)));
    connect(txt,SIGNAL(tabChanged(int)),matches,SLOT(slotChangeTab(int)));
    connect(txt,SIGNAL(tabClosed(int)),matches,SLOT(slotCloseTab(int)));
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
        if(!result.empty() && !txt->getText().isEmpty()){
            txt->setMatches(result);
            matches->setMatches(result);
        }
}

