#include "morphologyviewer.h"

const QString MorphologyViewer::TOOLTIP_TEMPLATE = " <h3>%1</h3>%2";

const QString MorphologyViewer::FORM_TEMPLATE = "<b>Base:</b>%1 <br>\
                                                                                                              <b> Attributes: </b><br> %2<hr>";

MorphologyViewer::MorphologyViewer(QWidget* parent):QTextEdit(parent)
{
    selectedFormat = new QTextCharFormat();
    selectedFormat->setUnderlineColor(Qt::red);
    selectedFormat->setUnderlineStyle(QTextCharFormat::UnderlineStyle::DashUnderline);
    selectedFormat->setFontItalic(true);
    selectedFormat->setForeground(Qt::red);
    setReadOnly(true);
    setMouseTracking(true);
    toolTipWidget = new QWidget(this);
}

QString MorphologyViewer::getToolTip(const QString &word) const
{
    QVector<WordForm> forms = morph.getWordInfo(word);
    QString formsString;
    for(const WordForm& frm : forms){
        QString attrStr;
        for(int i = 0; i < frm.attributeSets.size(); ++i){
            attrStr += frm.speechPart;
            if(!frm.attributeSets[i].empty()){
                 attrStr += ", ";
            }
            for(int j = 0; j < frm.attributeSets[i].size() - 1;++j) {
                attrStr += frm.attributeSets[i][j] + ", ";
            }
            if(!frm.attributeSets[i].empty()){
                attrStr += frm.attributeSets[i].back();
            }
            if(i != frm.attributeSets.size() - 1) attrStr += "<br>";
        }
        formsString +=  FORM_TEMPLATE.arg(frm.nf,attrStr);
    }

    return TOOLTIP_TEMPLATE.arg(word,formsString);
}

void MorphologyViewer::mousePressEvent(QMouseEvent *e)
{
    QTextCursor cursor = cursorForPosition(e->pos());
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = cursor.selectedText();
     if (!word.isEmpty()){
         this->setToolTip(getToolTip(word));
         toolTipPos = e->globalPos();
     }
}

void MorphologyViewer::mouseMoveEvent(QMouseEvent *e)
{
    ExtraSelection current;
    QTextCursor cursor = cursorForPosition(e->pos());
    cursor.select(QTextCursor::WordUnderCursor);
    current.cursor = cursor;
    current.format = *selectedFormat;
    setExtraSelections(QList<ExtraSelection>() << current);
}

void MorphologyViewer::mouseReleaseEvent(QMouseEvent *e)
{
    if(!QToolTip::isVisible()){
        QToolTip::showText(toolTipPos,toolTip(),toolTipWidget);
    }
}

bool MorphologyViewer::event(QEvent *e)
{
    if(e->type() == QEvent::ToolTip){
        e->ignore();
        return true;
    }
    return QTextEdit::event(e);
}
