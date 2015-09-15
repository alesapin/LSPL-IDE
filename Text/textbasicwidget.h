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
class TextTabEdit;
class TextBasicWidget : public QWidget
{
    Q_OBJECT
    TextTabEdit* textEdit;
    
public:
    explicit TextBasicWidget(QWidget *parent = 0);
    void saveTextFile(QString filename,int index = -1);
    void openTextFile(const QString& filename);
    bool maybeSave(QString filename,int index);
    void newTextFile();

signals:

public slots:
};

#endif // TEXTBASICWIDGET_H
