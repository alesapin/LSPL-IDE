#ifndef TEXTTABEDIT_H
#define TEXTTABEDIT_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <Text/maintextviewer.h>
#include <QList>
#include <QMessageBox>
#include <Text/textbasicwidget.h>
#include <Engine/patterncompiler.h>
#include <QMap>
class TextTabEdit : public QTabWidget
{
    Q_OBJECT
public:
    TextTabEdit(QWidget* parent= 0);
    QString addAnotherTab(const QString& filename,const QString& text);
    QString getCurrentText() const;
    QString getCurrentFile() const;
    QString getIndexText(int index) const;
    void highLightPatterns(const QStringList& patterns);
    void deHighlightPatterns(const QStringList& patterns);
    void setMatches(const PatternViewMap& m);
    void selectText(int start,int end);
    void clearSelection();
    void setReadOnly(bool f);

signals:
    /**
     * @brief tabWasClosed
     * Дополнительный сигнал для закрытия, т.к.
     * ввиду особенностей  closeTab, стандартный сигнал можно излучить
     * а вкладку не закрыть.
     */
    void tabWasClosed(int);
public slots:
    void closeTab(int index);
private:
    QMap<QString,QString> fileNamePath;
};

#endif // TEXTTABEDIT_H
