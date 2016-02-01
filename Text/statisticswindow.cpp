#include "statisticswindow.h"

StatisticsWindow::StatisticsWindow(const PatternViewMap &patterns, QWidget *parent):QDialog(parent)
{
    table = new QTableWidget(this);
    QHBoxLayout* l = new QHBoxLayout(this);
    setModal(true);
    table->setColumnCount(4);
    setWindowTitle("Statistics");
    table->setRowCount(patterns.keys().length());
    QStringList header;
    header<<"Имя"<<"Отрезков"<<"Сопоставлений" << "Вариантов";
    table->setHorizontalHeaderLabels(header);
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setContentsMargins(5,5,5,5);
    l->addWidget(table);
    setLayout(l);
    fillTable(patterns);

    resizeToFit();
}

StatisticsWindow::StatisticsWindow(QSharedPointer<utility::IntervalViewMap> data, QWidget *parent):QDialog(parent)
{
    table = new QTableWidget(this);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    QHBoxLayout* l = new QHBoxLayout(this);
    l->setStretch(0,1);
    setModal(true);
    table->setColumnCount(4);
    setWindowTitle("Статистика сопоставлений");
    QStringList header;
    header<<"Имя"<<"Отрезков"<<"Сопоставлений" << "Вариантов";
    table->setHorizontalHeaderLabels(header);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setContentsMargins(5,5,5,5);
    table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    l->addWidget(table);
    setLayout(l);
    fillTable(data);

}

void StatisticsWindow::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);
}



void StatisticsWindow::fillTable(const PatternViewMap &patterns)
{
    QStringList keys = patterns.keys();
    for(int i = 0; i < keys.length();++i){
        table->setItem(i,0,new QTableWidgetItem(keys[i]));
        table->setItem(i,1,new QTableWidgetItem(QString::number(patterns[keys[i]].length())));
        table->setItem(i,2,new QTableWidgetItem(QString::number(patterns[keys[i]].length())));
        table->setItem(i,3,new QTableWidgetItem(QString::number(countVariants(patterns[keys[i]]))));
    }
}

void StatisticsWindow::fillTable(QSharedPointer<utility::IntervalViewMap> data)
{
    QMap<QString,PatternStats> result;
    int segments = 0;
    int matches = 0;
    int variants = 0;
    for(utility::IntervalViewMap::iterator it = data->begin();it != data->end();++it){
        utility::IntervalMatch m = it->value;
        for(int i = 0 ;i <m.patterns.size();++i){
            if(result.contains(m.patterns[i])){
                result[m.patterns[i]].segments++;
                result[m.patterns[i]].variants+=m.variants[i];
            }else {
                result[m.patterns[i]] = {1,m.variants[i]};
            }

            variants+=m.variants[i];
        }
        segments++;
        matches+=m.patterns.size();
    }
    int i = 0;
    table->setRowCount(result.size()+1);
    for(const QString&val:result.keys()){
        table->setItem(i,0,new QTableWidgetItem(val));
        PatternStats stats = result[val];
        table->setItem(i,1,new QTableWidgetItem(QString::number(stats.segments)));
        table->setItem(i,2,new QTableWidgetItem(QString::number(stats.segments)));
        table->setItem(i,3,new QTableWidgetItem(QString::number(stats.variants)));
        i++;
    }
    table->setItem(i,0,new QTableWidgetItem("Всего"));
    table->setItem(i,1,new QTableWidgetItem(QString::number(segments)));
    table->setItem(i,2,new QTableWidgetItem(QString::number(matches)));
    table->setItem(i,3,new QTableWidgetItem(QString::number(variants)));
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
}

int StatisticsWindow::countVariants(const QVector<PatternCompiler::MatchRepr> &vect)
{
    int sum = 0;
    for(int i = 0 ; i< vect.size();++i){
        sum+= vect[i].variants;
    }
    return sum;
}

void StatisticsWindow::resizeToFit()
{

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    int w = table->horizontalHeader()->width()+4;
    for (int i = 0; i < table->columnCount(); i++){
        w += table->columnWidth(i);
    }
    int h = table->verticalHeader()->height()+4;
    for (int i = 0; i < table->rowCount(); i++){
       h += table->rowHeight(i);
    }
    resize(w,h);
}

