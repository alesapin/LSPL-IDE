#ifndef UTIL_HPP
#define UTIL_HPP
#include "Engine/patterncompiler.h"
#include "intervalrbtree.h"
#include <QSharedPointer>
#include <QVector>
#include <QPair>
#include <QString>
namespace utility{
    struct IntervalMatch{
        QString text;
        QVector<QString> patterns;
        QVector<QString> transforms;
        QVector<QString> params;

    };

    typedef IntervalRBTree<IntervalMatch> IntervalViewMap;

    QSharedPointer<IntervalViewMap> convertMatchesToIntervals(const PatternViewMap& m);
}
#endif // UTIL_HPP

