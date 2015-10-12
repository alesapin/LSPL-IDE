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
#include <Text/textmatchestable.h>
#include <QTableView>
#include <QTabWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <Engine/patterncompiler.h>
#include <Text/matcheswidget.h>
#include <QDomDocument>
class TextTabEdit;
class TextBasicWidget : public QMainWindow
{
    Q_OBJECT
    TextTabEdit* textEdit;
    QPushButton* analyze;
    MatchesWidget* matches;
    QTabWidget* matchTabs;
    PatternCompiler* compiler;


    void initTable();
    void initEditor();
    void initButton();
    QDomDocument toXml(PatternViewMap matches);

public:
    explicit TextBasicWidget(PatternCompiler* comp,QWidget *parent = 0);
    void saveTextFile(QString filename,int index = -1);
    void openTextFile(const QString& filename);
    bool maybeSave(QString filename,int index);
    void newTextFile();
    void saveMatches(QString filename);
    QDomDocument getXml();
    void setMatches(const PatternViewMap& matches);
    void highlighPatterns(const QStringList& patterns);
    void dehighlightPatterns(const QStringList& patterns);
    QString getCurrentFile() const ;
    QString getText() const ;
signals:
    void buttonClicked();
public slots:
    void clearMatches();
    void analyzeText();
    void selectFragment(int from,int to);
    void slotPatternUncheked(const QString& name);
    void slotPatternChecked(const QString& name);
    void slotTabChanged(int index);
};

#endif // TEXTBASICWIDGET_H
