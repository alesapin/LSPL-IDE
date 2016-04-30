#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H


#include "Text/textbasicwidget.h"
#include "Patterns/patternsbasicwidget.h"
#include "Engine/patterncompiler.h"
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
#include <QPushButton>
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
    static const QString CRASH_PATTERNS;
    static const QString CRASH_TEXT;

    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();
    TextBasicWidget *getTextWidget();
    PatternsBasicWidget *getPatternWidget();
    MatchesBasicWidget *getMatchesWidget();
    PatternCompiler *getPatternCompiler();
    QStringList getChoosenPatterns();
    void loadAfterCrash(const QString& textFileName, const QString& patternFileName);
    void createBackup() const;
private:
    PatternCompiler* compiler;
    TextBasicWidget* txt;
    PatternsBasicWidget* pattern;
    MatchesBasicWidget* matches;
    QFutureWatcher<QSharedPointer<PatternViewMap>>* watcher;
    QTimer *timeoutTimer;
    int timeOut;
    static const int DEFAULT_TIMEOUT;
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
    void slotTimeout();
    void slotProgress(int val);
};

#endif // CENTRALWIDGET_H
