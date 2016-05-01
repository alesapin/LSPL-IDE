#include "matchesbasicwidget.h"

MatchesBasicWidget::MatchesBasicWidget(QWidget* parent) : BasicWidget(parent,"Сопоставления")
{
    QWidget* wrapper = new QWidget();
    QVBoxLayout* lay = new QVBoxLayout(wrapper);
    QHBoxLayout* line = new QHBoxLayout();
    selectPattern = new QLabel("Выбрать шаблоны: ");
    QSizePolicy small(QSizePolicy::Preferred,QSizePolicy::Preferred);
    small.setHorizontalStretch(0);
    selectPattern->setSizePolicy(small);
    QSizePolicy big(QSizePolicy::Preferred,QSizePolicy::Preferred);
    big.setHorizontalStretch(5);
    list = new PatternSelectionList(this);
    list->setSizePolicy(big);
    selectAll = new QPushButton("Выбрать все");
    deselectAll = new QPushButton("Отменить все");
    selectAll->setEnabled(false);
    deselectAll->setEnabled(false);
    line->addWidget(selectPattern);
    line->addWidget(list);
    line->addWidget(selectAll);
    line->addWidget(deselectAll);
    table = new MatchesTable(this);
    lay->addLayout(line);
    lay->addWidget(table);
    lay->setContentsMargins(0,0,0,0);
    wrapper->setLayout(lay);
    wrapper->setContentsMargins(10,10,10,10);
    setWidget(wrapper);

    connect(table,SIGNAL(rowClicked(int,int)),this,SIGNAL(rowClicked(int,int)));
    connect(list,SIGNAL(patternUncheked(QString)),this,SLOT(slotPatternUnchecked(QString)));
    connect(list,SIGNAL(patternChecked(QString)),this,SLOT(slotPatternChecked(QString)));
    connect(selectAll,SIGNAL(clicked(bool)),this,SLOT(slotSelectAllClicked()));
    connect(deselectAll,SIGNAL(clicked(bool)),this,SLOT(slotDeselectAllClicked()));
}

void MatchesBasicWidget::setMatches(QSharedPointer<utility::IntervalViewMap> patterns,const QStringList& names)
{
    table->setMatches(patterns);
    if(names.empty()){
        QSet<QString> n;
        for(auto itr = patterns->begin(); itr != patterns->end(); ++itr){
            for(const QString &name : itr->value.patterns){
                n.insert(name);
            }
        }
        list->clear();
        list->addItems(n.toList());
    }else{
        list->clear();
        list->addItems(names);
    }
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
    selectAll->setEnabled(true);
    deselectAll->setEnabled(true);
    list->slotSetUserCheckable(true);
}

void MatchesBasicWidget::slotSelectAllClicked()
{
    list->slotSelectAll();
    list->slotSetUserCheckable(false);
    selectAll->setEnabled(false);
    deselectAll->setEnabled(false);
    table->slotShowAll();
    emit showAll();
}

void MatchesBasicWidget::slotDeselectAllClicked()
{
    list->slotDeselectAll();
    list->slotSetUserCheckable(false);
    selectAll->setEnabled(false);
    deselectAll->setEnabled(false);
    table->slotHideAll();
    emit hideAll();
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
    list->clearCurrent();
}

QDomDocument MatchesBasicWidget::getXml() const
{
    return utility::matchesToXml(table->getCurrentMatches());
}


void MatchesBasicWidget::slotCloseTab(int index)
{
    table->closeTab(index);
    list->closeTab(index);
    for(const QString& pattern:list->unchekedItems()) {
        table->hidePattern(pattern);
    }
}

void MatchesBasicWidget::slotPatternUnchecked(const QString &s)
{
    selectAll->setEnabled(false);
    deselectAll->setEnabled(false);
    table->hidePattern(s);
    emit patternWasUnchecked(s);
}

void MatchesBasicWidget::slotPatternChecked(const QString &s)
{
    selectAll->setEnabled(false);
    deselectAll->setEnabled(false);
    table->showPattern(s);
    emit patternWasChecked(s);
}
