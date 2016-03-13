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
#include <QHeaderView>
#include <QTableView>
class PatternsList : public QTableView
{
    Q_OBJECT
public:
    PatternsList(PatternCompiler* compiler,QWidget* parent = 0);
    QStringList getCompiledPatterns() const;
    QStringList getCompiledPatternsNames() const;
    void clearAll();
    void addPattern(const QString& text);
    void addPatterns(const QStringList& patterns);
protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
private:
    void dragMoveEvent( QDragMoveEvent* pEvent) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent* pEvent) Q_DECL_OVERRIDE;
    void dropEvent( QDropEvent* pEvent) Q_DECL_OVERRIDE;

    PatternListModel* myModel;
    PatternCompiler* compiler;
    QAction *removeAction;
    QAction *editAction;
    PatternItemDelegate* delegate;

public slots:
    void slotCompilePatterns();
    void slotRemovePattern();
    void slotEditPattern();
    void slotTextEntered();
signals:
    void editPatternSignal(const QString& text);
};

#endif // PATTERNSLIST_H
