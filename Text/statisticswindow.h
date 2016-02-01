#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include "Engine/patterncompiler.h"
#include <QSharedPointer>
#include "Utils/util.hpp"
#include <QResizeEvent>
class StatisticsWindow : public QDialog
{
public:
    StatisticsWindow(const PatternViewMap& patterns,QWidget* parent=0);
    StatisticsWindow(QSharedPointer<utility::IntervalViewMap> data, QWidget* parent=0);
protected:
    void resizeEvent(QResizeEvent * e) Q_DECL_OVERRIDE;
private:
    struct PatternStats {
        int segments;
        int variants;
    };

    QTableWidget* table;
    void fillTable(const PatternViewMap& patterns);
    void fillTable(QSharedPointer<utility::IntervalViewMap> data);
    int countVariants(const QVector<PatternCompiler::MatchRepr>& vect);

    void resizeToFit();
};

#endif // STATISTICSWINDOW_H
