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
    QString form;
    QString nf;
    QVector<QString> attributes;
};

class MorphAn
{
private:
    QSharedPointer<lspl::morphology::AotMorphology> morph;
public:
    QString getSpeechPart(const QString& word);
    MorphAn();
};

#endif // MORPHAN_H
