#include "centralwidget.h"
#include <QDebug>
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    compiler = new PatternCompiler();
    txt = new TextBasicWidget(compiler,this);
    pattern = new PatternsBasicWidget(compiler,this);
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

QStringList CentralWidget::getChoosenPatterns()
{
    return pattern->getChoosenPatterns();
}

void CentralWidget::analyze()
{
    QStringList patternNames = pattern->getChoosenPatterns();
    PatternViewMap result =  compiler->analyzeText(patternNames,txt->getText());
    txt->setMatches(result);
//    for(QString name:patternNames){
//        QVector<PatternCompiler::MatchRepr> pattrnResult = result[name];
//        pattern->setPatternValues(name,pattrnResult.size(),pattrnResult.size(),-1);
//    }
}

