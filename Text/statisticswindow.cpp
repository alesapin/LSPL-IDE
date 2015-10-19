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
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setContentsMargins(5,5,5,5);
    l->addWidget(table);
    setLayout(l);
//    table->horizontalHeader()->setStretchLastSection(true);
//    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    fillTable(patterns);

    resizeToFit();
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

