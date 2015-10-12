#ifndef PATTERNCOMPLOGBAR_H
#define PATTERNCOMPLOGBAR_H
#include <QTextEdit>
#include <QTabBar>
class PatternCompLogBar:public QTextEdit
{
public:
    PatternCompLogBar(QWidget* parent = 0);
    void clear();
};

#endif // PATTERNCOMPLOGBAR_H
