#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include <QStatusBar>
#include <QProgressBar>
enum ProgressStates{
    READY_STATE,ENGINE_STATE,HIGHLIGHT_STATE,PATTERN_STATE
};

class BottomBar : public QStatusBar
{
public:
    BottomBar(QWidget* parent = 0);
private:
    static const QString READY;
    static const QString ENGINE_WORKING;
    static const QString HIGHLIGHT_PROCESSING;
    static const QString PATTERN_COMPILING;
public slots:
    void slotTextAnalyzing(QString);

};

#endif // BOTTOMBAR_H
