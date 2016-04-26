#include "morphan.h"

 QVector<WordForm> MorphAn::getWordInfo(const QString &word) const
{
    QVector<WordForm> result;
    std::auto_ptr<boost::ptr_vector<lspl::morphology::WordForm>> r = morph->getWordForms(utility::covertToWinCp(word));
    for(int i = 0;i<r->size();++i){
        WordForm current = convertWordForm(r->operator [](i));
        result.push_back(current);
    }
    return result;
}

WordForm MorphAn::convertWordForm(const lspl::morphology::WordForm& wf) const
{
    using lspl::text::attributes::AttributeKey;
    using lspl::text::attributes::AttributeValue;
    WordForm result;
    result.speechPart = utility::convertToUnicode(wf.getSpeechPart().getName());
    result.nf = utility::convertToUnicode(wf.getBase());
    for(int i = 0;i<wf.getAttributeSetCount();++i){
        QVector<QString> currentSet;
        for(int j = 1;j<AttributeKey::count();++j){
            AttributeKey currkey(j);
            if(currkey == AttributeKey::BASE || currkey == AttributeKey::STEM) continue;
            AttributeValue val = wf.getAttribute(i,currkey);
            if (val != AttributeValue::UNDEFINED) {
                    QString stringVal = utility::convertToUnicode(/*currkey.getName() +" : "+*/val.getString());
                    currentSet.push_back(stringVal);
              }
        }
        result.attributeSets.push_back(currentSet);
    }
    return result;

}

MorphAn::MorphAn()
{
    lspl::morphology::AotMorphology * aot = dynamic_cast<lspl::morphology::AotMorphology*>(&lspl::morphology::Morphology::instance());
    morph = QSharedPointer<lspl::morphology::AotMorphology>(aot);
}
