#ifndef UTIL_HPP
#define UTIL_HPP
#include "Engine/patterncompiler.h"
#include "intervalrbtree.h"
#include <QSharedPointer>
#include <QVector>
#include <QPair>
#include <QString>
#include <QDomDocument>
#include <charsetdetect.h>
#include <QFile>
namespace utility{
    struct IntervalMatch{
        QString text;
        QVector<QString> patterns;
        QVector<QString> transforms;
        QVector<QString> params;
        QVector<int> variants;

    };
    QPair<QString, QString> splitPattern(const QString &pattern);
    typedef IntervalRBTree<IntervalMatch> IntervalViewMap;
    QDomDocument toXml(const PatternViewMap& matches);
    QDomDocument toXml(QSharedPointer<IntervalViewMap> matches);
    QSharedPointer<IntervalViewMap> convertMatchesToIntervals(const PatternViewMap& m);
    const char *codecNameForText(QFile& arr);
}
#endif // UTIL_HPP

