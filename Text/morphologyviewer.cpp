#include "morphologyviewer.h"

MorphologyViewer::MorphologyViewer(QWidget* parent):QTextEdit(parent)
{
    selectedFormat = new QTextCharFormat();
    selectedFormat->setUnderlineColor(Qt::red);
    selectedFormat->setUnderlineStyle(QTextCharFormat::UnderlineStyle::DashUnderline);
    selectedFormat->setFontItalic(true);
    selectedFormat->setForeground(Qt::red);
    setReadOnly(true);
    setMouseTracking(true);
    //morph.getSpeechPart("красотой");
}

QString MorphologyViewer::getToolTip(const QString &word) const
{
    return word;
}

void MorphologyViewer::mousePressEvent(QMouseEvent *e)
{
    QTextCursor cursor = cursorForPosition(e->pos());
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = cursor.selectedText();
     if (! word.isEmpty()){
         QToolTip::showText(e->globalPos(), getToolTip(word));
     }
}

void MorphologyViewer::mouseMoveEvent(QMouseEvent *e)
{
    QTextCursor cursor = cursorForPosition(e->pos());
    cursor.select(QTextCursor::WordUnderCursor);
    cursor.setCharFormat(*selectedFormat);
}
