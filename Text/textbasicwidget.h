#ifndef TEXTBASICWIDGET_H
#define TEXTBASICWIDGET_H

#include <QWidget>
#include <Text/maintextviewer.h>
#include <QMainWindow>
#include <QGridLayout>
#include <Interface/topmenu.h>
#include <Text/texttabedit.h>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QCursor>
#include <QTextStream>
#include <QFileDialog>
#include <QStringList>
#include <QPushButton>
#include <QTableView>
#include <QTabWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include "Engine/patterncompiler.h"
#include <QDomDocument>
#include "statisticswindow.h"
#include "basicwidget.h"
class TextTabEdit;
class TextBasicWidget : public BasicWidget
{
    Q_OBJECT
    TextTabEdit* textEdit;
    QPushButton* analyze;
    QPushButton* statistics;
    QPushButton* edit;



    void initEditor(QMainWindow* wrapper);
    void initButtons(QMainWindow *wrapper);

public:
    explicit TextBasicWidget(QWidget *parent = 0);
    void saveTextFile(QString filename,int index = -1);
    void openTextFile(const QString& filename);
    bool maybeSave(QString filename,int index);
    void newTextFile();
    void saveMatches(QString filename);
    void setMatches(const PatternViewMap& matches);
    void highlighPatterns(const QStringList& patterns);
    void dehighlightPatterns(const QStringList& patterns);
    QString getCurrentFile() const ;
    QString getText() const ;
signals:
    void buttonClicked();
    void tabClosed(int);
    void tabChanged(int);
public slots:
    void slotClearMatches();
    void slotAnalyzeText();
    void slotEditEnable();
    void slotShowStatistics();
    void slotSelectFragment(int from,int to);
    void slotPatternUncheked(const QString& name);
    void slotPatternChecked(const QString& name);
};

#endif // TEXTBASICWIDGET_H
