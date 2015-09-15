#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Text/maintextviewer.h>
#include <Text/textbasicwidget.h>
#include <Patterns/patternsbasicwidget.h>
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
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
