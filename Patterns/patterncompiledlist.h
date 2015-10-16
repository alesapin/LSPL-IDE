#ifndef PATTERNCOMPILEDLIST_H
#define PATTERNCOMPILEDLIST_H

#include <QWidget>
#include <QListView>
#include <QAbstractListModel>
#include <QStringListModel>
#include <QSet>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QListWidget>
#include <QMap>
#include <QLineEdit>
#include <QSizePolicy>
#include <QSize>
#include <QFontMetrics>
class PatternCompiledList : public QListWidget
{
    Q_OBJECT
public:
    PatternCompiledList(QWidget* parent=0);
    void addPattern(const QString& pattern);
    void addPatterns(const QStringList& patterns);
    QStringList getCompiledPatterns() const;
    QStringList getPatternsForCompile() const;
    void clearAll();
    QString getText() const ;
    void setUncompiledPatterns(const QStringList& patterns);
signals:

public slots:
    void showContextMenu(const QPoint& p);
    void editPattern();
    void removePattern();
    void patternChanged(QListWidgetItem * item);
private:
    void addUncompiledPattern(const QString& name);
    QString getName(QString pattern);
    int getItemHeight(QListWidgetItem* it);
    void shrinkToFit();
    QListWidgetItem *getItem(QString pattern);
    QStringListModel* model;
    QStringList compiledPatterns;
    QMap<QString,QString> nonCompiledPatterns;
    QAction* editPatternAct;
    QAction* removePatternAct;
    QFontMetrics* metric;

};

#endif // PATTERNCOMPILEDLIST_H
