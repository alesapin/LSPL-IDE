#ifndef PATTEREDITOR_H
#define PATTEREDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QVector>
class PatternEditor:public QPlainTextEdit
{
    Q_OBJECT
public:
    PatternEditor(QWidget* parent=0);
    QStringList getCompiledPatterns() const;
    void addPatterns(const QStringList& patterns);
    void addPattern(const QString& pattern);
private:
    QSet<QString> compiledPatterns;
};

#endif // PATTEREDITOR_H
