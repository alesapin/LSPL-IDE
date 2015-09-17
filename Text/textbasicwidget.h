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
#include <Text/textmatchesmodel.h>
#include <QTableView>
#include <QTabWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <Engine/patterncompiler.h>
class TextTabEdit;
class TextBasicWidget : public QWidget
{
    Q_OBJECT
    TextTabEdit* textEdit;
    QPushButton* analyze;
    TextMatchesModel* matchesModel;
    QTableView* matchesTable;
    QTabWidget* matchTabs;
    PatternCompiler* compiler;

    void initTable();
    void initEditor();
    void initButton();
public:
    explicit TextBasicWidget(PatternCompiler* comp,QWidget *parent = 0);
    void saveTextFile(QString filename,int index = -1);
    void openTextFile(const QString& filename);
    bool maybeSave(QString filename,int index);
    void newTextFile();
    void setMatches(const QVector<PatternCompiler::MatchRepr>& matches);
    QString getText();
signals:
    void buttonClicked();
public slots:
    void analyzeText();
};

#endif // TEXTBASICWIDGET_H
