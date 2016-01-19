#ifndef PATTERNSLIST_H
#define PATTERNSLIST_H

#include <QListView>
#include "Engine/patterncompiler.h"
#include "patternlistmodel.h"
#include <QModelIndex>
#include "patternitemdelegate.h"
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QAction>
class PatternsList : public QListView
{
    Q_OBJECT
public:
    PatternsList(PatternCompiler* compiler,QWidget* parent = 0);
    QStringList getCompiledPatterns() const;
    void clearAll();
    void addPattern(const QString& text);
    void addPatterns(const QStringList& patterns);

private:
    void dragMoveEvent( QDragMoveEvent* pEvent);
    void dragEnterEvent(QDragEnterEvent* pEvent);
    void dropEvent( QDropEvent* pEvent);
    PatternListModel* myModel;
    PatternCompiler* compiler;
    QAction *removeAction;
    QAction *editAction;
public slots:
    void slotCompilePatterns();
    void slotRemovePattern();
    void slotEditPattern();
signals:
    void editPatternSignal(const QString& text);
};

#endif // PATTERNSLIST_H
