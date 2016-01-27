#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <Text/textbasicwidget.h>
#include <Patterns/patternsbasicwidget.h>
#include <Engine/patterncompiler.h>
#include "Matches/matchesbasicwidget.h"
#include <QMainWindow>
#include <QGridLayout>
#include <Interface/topmenu.h>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QCursor>
#include <QTextStream>
#include <QVector>
#include <QString>
#include <QSplitter>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include "Interface/bottombar.h"
class TextBasicWidget;
class CentralWidget : public QWidget
{
    Q_OBJECT
public:

    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();
    TextBasicWidget *getTextWidget();
    PatternsBasicWidget *getPatternWidget();
    MatchesBasicWidget *getMatchesWidget();
    QStringList getChoosenPatterns();
private:
    PatternCompiler* compiler;
    TextBasicWidget* txt;
    PatternsBasicWidget* pattern;
    MatchesBasicWidget* matches;
    QFutureWatcher<QSharedPointer<PatternViewMap>>* watcher;
signals:
    void statusReady();
    void statusEngine();
    void statusHighlighting();
    void statusCompiling();
    void statusCanceled();
public slots:
    void slotAnalyze();
    void slotDisplay();
    void slotEdit();
    void slotProgress(int val);
};

#endif // CENTRALWIDGET_H
