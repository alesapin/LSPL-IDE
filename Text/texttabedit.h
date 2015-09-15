#ifndef TEXTTABEDIT_H
#define TEXTTABEDIT_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <Text/maintextviewer.h>
#include <QList>
#include <QMessageBox>
#include <Text/textbasicwidget.h>
class TextTabEdit : public QTabWidget
{
    Q_OBJECT
public:
    TextTabEdit(QWidget* parent= 0);
    void addAnotherTab(const QString& filename,const QString& text);
    QString getCurrentText() const;
    QString getIndexText(int index) const;
public slots:
    void closeTab(int index);
};

#endif // TEXTTABEDIT_H
