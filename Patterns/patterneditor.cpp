#include "patterneditor.h"



PatternEditor::PatternEditor(QWidget *parent)
{

}

QStringList PatternEditor::getCompiledPatterns() const
{
    return compiledPatterns.values();
}

void PatternEditor::addPatterns(const QStringList &patterns)
{
    for(const QString& pattern: patterns){
        compiledPatterns.insert(pattern);
    }
}

void PatternEditor::addPattern(const QString &pattern)
{
    compiledPatterns.insert(pattern);
}
