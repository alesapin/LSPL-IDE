#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <Text/textbasicwidget.h>
#include <Patterns/patternsbasicwidget.h>
#include <Engine/patterncompiler.h>
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
class TextBasicWidget;
class CentralWidget : public QWidget
{
    Q_OBJECT
public:

    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();
    TextBasicWidget *getTextWidget();
    QVector<QString> getChoosenPatterns();
private:
    PatternCompiler* compiler;
    TextBasicWidget* txt;
    PatternsBasicWidget* pattern;
signals:

public slots:
    void analyze();
};

#endif // CENTRALWIDGET_H
