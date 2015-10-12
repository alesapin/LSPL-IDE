#ifndef PATTEREDITOR_H
#define PATTEREDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QVector>
#include <QPainter>
#include <QTextBlock>
#include <QFontMetrics>
class PatternEditor:public QTextEdit
{
    Q_OBJECT
public:
    PatternEditor(QWidget* parent=0);
    QStringList getCompiledPatterns() const;
    void addPatterns(const QStringList& patterns);
    void addPattern(const QString& pattern);
    void selectPattern(const QString& pattern);
    QString getText() const;
protected:
   // virtual void paintEvent(QPaintEvent *_event);
    virtual void keyPressEvent(QKeyEvent *e);
private:
    void colorizeText();
    int getPatternPos(const QString& patternName);
    QTextCursor* cursor;
    QTextCharFormat* fmtCompiled;
    QTextCharFormat* fmtError;
    QMap<QString,QString> compiledPatterns;
    QMap<QString,QPair<int,int>> patterns;
    QFontMetrics* metrics;
    QString getPatternByLine(int num);
    int getCursorLine();
    static const QString NOT_A_PATTERN;
public slots:
    void clearAll();
    void checkEditable();
};

#endif // PATTEREDITOR_H
