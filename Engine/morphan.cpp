#include "morphan.h"

 QVector<WordForm> MorphAn::getWordInfo(const QString &word) const
{
    QVector<WordForm> result;
    std::auto_ptr<boost::ptr_vector<lspl::morphology::WordForm>> r = morph->getWordForms(utility::covertToWinCp(word));
    for(int i = 0;i<r->size();++i){
        convertWordForm(r->operator [](i));
    }
    return result;
}

WordForm MorphAn::convertWordForm(const lspl::morphology::WordForm& wf) const
{
    using lspl::text::attributes::AttributeKey;
    using lspl::text::attributes::AttributeValue;
    WordForm result;
    result.nf = utility::convertToUnicode(wf.getBase());
    for(int i = 0;i<wf.getAttributeSetCount();++i){
        for(int j = 1;j<AttributeKey::count();++j){
            AttributeKey currkey(j);
                AttributeValue val = wf.getAttribute(i,currkey);
                if(val != AttributeValue::UNDEFINED){
                    QString stringVal = utility::convertToUnicode(val.getName()+" : "+val.getString());
                    qDebug() << stringVal;
                    result.attributes.push_back(stringVal);
                }

        }
    }
    return result;

}

MorphAn::MorphAn()
{
    lspl::morphology::AotMorphology * aot = dynamic_cast<lspl::morphology::AotMorphology*>(&lspl::morphology::Morphology::instance());
    morph = QSharedPointer<lspl::morphology::AotMorphology>(aot);
}
