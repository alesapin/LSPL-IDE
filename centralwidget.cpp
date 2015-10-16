#include "centralwidget.h"
#include <QDebug>
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    compiler = new PatternCompiler();
    txt = new TextBasicWidget(compiler,this);
    pattern = new PatternsBasicWidget(compiler,this);
    QSizePolicy large(QSizePolicy::Expanding,QSizePolicy::Expanding);
    large.setHorizontalStretch(3);
    txt->setSizePolicy(large);
    QSizePolicy small(QSizePolicy::Expanding,QSizePolicy::Expanding);
    small.setHorizontalStretch(1);
    pattern->setSizePolicy(small);
    QHBoxLayout* lay = new QHBoxLayout(this);
    QSplitter* split1 = new QSplitter();
    split1->addWidget(txt);
    split1->addWidget(pattern);
    connect(txt,SIGNAL(buttonClicked()),this,SLOT(analyze()));
    lay->addWidget(split1);
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

QStringList CentralWidget::getChoosenPatterns()
{
    return pattern->getChoosenPatterns();
}

void CentralWidget::analyze()
{
    QStringList patternNames = pattern->getChoosenPatterns();
    try{
        PatternViewMap result =  compiler->analyzeText(patternNames,txt->getText());
        if(!result.empty() && !txt->getText().isEmpty()){
            txt->setMatches(result);
        }
    }catch(lspl::patterns::PatternBuildingException e){
        pattern->addLog("Exception while building generated pattern");
    }

//    for(QString name:patternNames){
//        QVector<PatternCompiler::MatchRepr> pattrnResult = result[name];
//        pattern->setPatternValues(name,pattrnResult.size(),pattrnResult.size(),-1);
//    }
}

