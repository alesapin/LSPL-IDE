#ifndef XMLPROCESSOR_H
#define XMLPROCESSOR_H

#include "intervalrbtree.h"
#include <QSharedPointer>
#include <QDomDocument>
#include <QFile>
#include "util.hpp"
namespace utility {
class XmlProcessor
{
private:
    QDomDocument maindoc;
    QStringList patterns;
    QString text;
    QSharedPointer<IntervalViewMap> matches;

    bool readMatches(const QDomElement& matchElem);
    bool readText(const QDomElement& textElem);
    bool readPatterns(const QDomElement& patElem);

    bool writeMatches(QDomElement& matchElem);
    bool writeText(QDomElement& textElem);
    bool writePatterns(QDomElement& patElem);

public:
    XmlProcessor(const QString& filename);
    XmlProcessor();

    QStringList getPatterns() const { return patterns;}
    QString getText() const { return text; }
    QSharedPointer<IntervalViewMap> getMatches() const { return matches; }



    void setPatterns(const QStringList& patterns) {
        this->patterns = patterns;
    }

    void setText(const QString& text) {
        this->text = text;
    }

    void setMatches(const QSharedPointer<IntervalViewMap> matches ) {
        this->matches = matches;
    }

    bool readFile(const QString& name);
    bool writeFile(const QString& name);

    static const QString MAIN_TAG;
    static const QString TEXT_TAG;
    static const QString PATTERNS_TAG;
    static const QString PATTERN_TAG;
    static const QString MATCHES_TAG;
    static const QString MATCH_TAG;
    static const QString MATCH_START_ATTR;
    static const QString MATCH_END_ATTR;
    static const QString MATCH_TEXT_ATTR;
    static const QString MATCH_PATTERN_TAG;
    static const QString MATCH_PATTERN_NAME_TAG;
    static const QString MATCH_PATTERN_TRANS_TAG;
    static const QString MATCH_PATTERN_PARAMS_TAG;
    static const QString MATCH_VARIANT_TAG;

};
}
#endif // XMLPROCESSOR_H
