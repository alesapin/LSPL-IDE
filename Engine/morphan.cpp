#include "morphan.h"

QString MorphAn::getSpeechPart(const QString &word)
{
    boost::ptr_vector<lspl::morphology::WordForm> result;
    morph->appendWordForms(utility::covertToWinCp(word),result);
    for(int i = 0;i<result.size();++i){
        qDebug()<<utility::convertToUnicode(result[i].getBase());
    }
    return utility::convertToUnicode(result[0].getSpeechPart().getAbbrevation());
}

MorphAn::MorphAn()
{
    lspl::morphology::AotMorphology * aot = dynamic_cast<lspl::morphology::AotMorphology*>(&lspl::morphology::Morphology::instance());
    morph = QSharedPointer<lspl::morphology::AotMorphology>(aot);
}
