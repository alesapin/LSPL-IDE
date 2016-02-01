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
#include "Utils/util.hpp"
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
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    void addUncompiledPattern(const QString& pattern);
    void addUncompiledPatterns(const QStringList& patterns);
    void updatePattern(const QString& patternName, QString text);
    QStringList getUncompiledPatterns() const;
    QStringList getCompiledPatterns() const;
    void clearAll();
    bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    bool insertRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QMimeData *mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;
    void swapRows(int first,int second);
private:

    QWidget* par;
    QFontMetrics* metrics;
    bool recompileAll;
    //Данные в строках
    QList<ListItem> rowData;
    void setPatternsUncompiled();

};
Q_DECLARE_METATYPE(PatternListModel::ListItem)



#endif // PATTERNLISTMODEL_H
