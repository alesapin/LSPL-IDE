#include "matchesbasicwidget.h"

MatchesBasicWidget::MatchesBasicWidget(QWidget* parent) : BasicWidget(parent,"Сопоставления")
{
    QWidget* wrapper = new QWidget();
    QVBoxLayout* lay = new QVBoxLayout(wrapper);
    QHBoxLayout* line = new QHBoxLayout();
    selectPattern = new QLabel("Select Patterns: ");
    QSizePolicy small(QSizePolicy::Preferred,QSizePolicy::Preferred);
    small.setHorizontalStretch(0);
    selectPattern->setSizePolicy(small);
    QSizePolicy big(QSizePolicy::Preferred,QSizePolicy::Preferred);
    big.setHorizontalStretch(5);
    list = new PatternSelectionList(this);
    list->setSizePolicy(big);
    line->addWidget(selectPattern);
    line->addWidget(list);
    table = new MatchesTable(this);
    lay->addLayout(line);
    lay->addWidget(table);
    lay->setContentsMargins(0,0,0,0);
    wrapper->setLayout(lay);
    wrapper->setContentsMargins(10,10,10,10);
    setWidget(wrapper);
    connect(table,SIGNAL(rowClicked(int,int)),this,SLOT(slotRowClicked(int,int)));
    connect(list,SIGNAL(patternUncheked(QString)),this,SLOT(slotPatternUnchecked(QString)));
    connect(list,SIGNAL(patternChecked(QString)),this,SLOT(slotPatternChecked(QString)));
}

void MatchesBasicWidget::setMatches(QSharedPointer<utility::IntervalViewMap> patterns,const QStringList& names)
{
    table->setMatches(patterns);
    list->clear();
    list->addItems(names);
    blockSignals(true);
    list->checkAll();
    blockSignals(false);
}

void MatchesBasicWidget::slotChangeTab(int index)
{
    if(index != -1){
        table->changeTab(index);
        list->changeTab(index);
        for(const QString& pattern:list->unchekedItems()) {
            table->hidePattern(pattern);
        }
    } else {
        table->clear();
        list->clearAll();
    }
}

void MatchesBasicWidget::slotEnableChecking()
{
    qDebug() << "CALLED";
    list->setUserCheckable(true);
}

void MatchesBasicWidget::saveMatches(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(filename)
                                 .arg(file.errorString()));
            return ;
     }
    QTextStream out(&file);
   #ifndef QT_NO_CURSOR
       QApplication::setOverrideCursor(Qt::WaitCursor);
   #endif
        out << getXml().toString();
   #ifndef QT_NO_CURSOR
       QApplication::restoreOverrideCursor();
#endif
}


QSharedPointer<utility::IntervalViewMap> MatchesBasicWidget::getSelectedMatches() const
{
    return table->getCurrentMatches();
}

void MatchesBasicWidget::slotClear()
{
    table->clear();
    list->clearAll();
}

QDomDocument MatchesBasicWidget::getXml()
{
    return utility::toXml(table->getCurrentMatches());
}


void MatchesBasicWidget::slotCloseTab(int index)
{
    table->closeTab(index);
    list->closeTab(index);
    for(const QString& pattern:list->unchekedItems()) {
        table->hidePattern(pattern);
    }
}



void MatchesBasicWidget::slotRowClicked(int s, int e)
{
    emit rowClicked(s,e);
}

void MatchesBasicWidget::slotPatternUnchecked(const QString &s)
{
    table->hidePattern(s);
    emit patternWasUnchecked(s);
}

void MatchesBasicWidget::slotPatternChecked(const QString &s)
{
    table->showPattern(s);
    emit patternWasChecked(s);
}
