#ifndef DELEGATETEXTEDIT_H
#define DELEGATETEXTEDIT_H
#include <QTextEdit>
#include "patternhighlighter.h"

class DelegateTextEdit : public QTextEdit{
    Q_OBJECT
public:
    explicit DelegateTextEdit(QWidget* parent = 0);
signals:
    void editingFinished();
protected:
    void keyPressEvent( QKeyEvent * e ) Q_DECL_OVERRIDE;
};

#endif // DELEGATETEXTEDIT_H
