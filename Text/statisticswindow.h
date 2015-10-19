#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include "Engine/patterncompiler.h"
class StatisticsWindow : public QDialog
{
public:
    StatisticsWindow(const PatternViewMap& patterns,QWidget* parent=0);
private:
    QTableWidget* table;
    void fillTable(const PatternViewMap& patterns);
    int countVariants(const QVector<PatternCompiler::MatchRepr>& vect);
    void resizeToFit();
};

#endif // STATISTICSWINDOW_H
