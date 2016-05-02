#ifndef PATTERNSBASICWIDGET_H
#define PATTERNSBASICWIDGET_H

#include <QWidget>
#include <Patterns/patterneditor.h>
#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QCursor>
#include <QTextStream>
#include <QFileDialog>
#include <QStringList>
#include <QPushButton>
#include <Engine/patterncompiler.h>
#include <QTableView>
#include <QHeaderView>
#include <QSplitter>
#include <QDockWidget>
#include <QToolBar>
#include <QFrame>
#include <QTextEdit>
#include <QScrollArea>
#include "basicwidget.h"
#include "patternslist.h"
#include "Utils/util.hpp"
class PatternsBasicWidget : public BasicWidget
{
    Q_OBJECT
private:
    PatternEditor* editor;
    QPushButton* compileButton;
    QPushButton* addButton;
    QPushButton* clearAllButton;
    PatternCompiler* comp;
    PatternsList* list;
    void initPatternEditor();
public:
    explicit PatternsBasicWidget(PatternCompiler* compiler,QWidget *parent = 0);
    QStringList getChoosenPatternsNames() const;
    void importPatterns(QString filename);
    void exportPatterns(QString filename);
    QString getPatternsAsText() const;
    QStringList getPatterns() const;
    void loadPatterns(const QStringList& patterns);
signals:

public slots:
    void slotClearPatterns();
    void slotAddPattern();
};

#endif // PATTERNSBASICWIDGET_H
