#ifndef MORPHAN_H
#define MORPHAN_H

#include <lspl/morphology/WordForm.h>
#include <lspl/morphology/AotMorphology.h>
#include <lspl/morphology/Morphology.h>
#include <QString>
#include <QVector>
#include "Utils/util.hpp"
#include <QSharedPointer>
#include <boost/ptr_container/ptr_vector.hpp>
struct WordForm {
    QString nf;
    QString speechPart;
    QVector<QVector<QString>> attributeSets;
};

class MorphAn
{
private:
    lspl::morphology::AotMorphology* morph;
    WordForm convertWordForm(const lspl::morphology::WordForm& result) const;

public:
    QVector<WordForm> getWordInfo(const QString& word) const;
    MorphAn();
};

#endif // MORPHAN_H
