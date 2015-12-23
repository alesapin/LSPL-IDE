#ifndef PATTERNLISTMODEL_H
#define PATTERNLISTMODEL_H
#include <QAbstractListModel>
#include <QObject>
#include <QSet>
#include <QVariant>
#include <QList>
#include <QPair>
#include <QColor>
#include <QFontMetrics>
#include <QWidget>
#include <QMimeData>
class PatternListModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum CompilationState{
       Compiled,UnCompiled,FailCompiled
    };


    struct ListItem{
        QString name;
        QString text;
        CompilationState state;
        QString compilationOutput;
        bool operator==(const ListItem& o) const{
            return name == o.name;
        }
    };

    PatternListModel(QObject* parent=0);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    void addUncompiledPattern(const QString& pattern);
    void addUncompiledPatterns(const QStringList& patterns);
    void updatePattern(const QString& patternName, QString text);
    QStringList getUncompiledPatterns() const;
    QStringList getCompiledPatterns() const;
    void clearAll();
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool insertRows(int row, int count, const QModelIndex &parent);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    void swapRows(int first,int second);
private:
    QPair<QString,QString> splitPattern(const QString& pattern);

    QWidget* par;
    QFontMetrics* metrics;
    bool recompileAll;
    //Данные в строках
    QList<ListItem> rowData;
    void setPatternsUncompiled();
public slots:
    void slotRowMoved(const QModelIndex & sourceParent, int sourceStart, int sourceEnd,
                      const QModelIndex & destinationParent, int destinationRow);
};
Q_DECLARE_METATYPE(PatternListModel::ListItem)



#endif // PATTERNLISTMODEL_H
