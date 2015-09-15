#ifndef PATTERNSBASICWIDGET_H
#define PATTERNSBASICWIDGET_H

#include <QWidget>
#include <Patterns/patterneditortab.h>
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
class PatternsBasicWidget : public QWidget
{
    Q_OBJECT
private:
    PatternEditorTab* editorTab;
    QPushButton* compileButton;
public:
    explicit PatternsBasicWidget(QWidget *parent = 0);

signals:

public slots:
};

#endif // PATTERNSBASICWIDGET_H
