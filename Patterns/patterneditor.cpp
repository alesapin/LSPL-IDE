#include "patterneditor.h"
#include <QDebug>

const QString PatternEditor::NOT_A_PATTERN = "NoT_A_PaTtern";
PatternEditor::PatternEditor(QWidget *parent):QTextEdit(parent)
{
    fmtCompiled = new QTextCharFormat();
    fmtError = new QTextCharFormat();
    cursor = new QTextCursor(this->document());
    fmtCompiled->setBackground(Qt::green);
    metrics = new QFontMetrics(font());
    setUpdatesEnabled(true);
    setMinimumHeight(300);
    this->setAlignment(Qt::AlignLeft);
    connect(this,SIGNAL(cursorPositionChanged()),this,SLOT(checkEditable()));
}

QStringList PatternEditor::getCompiledPatterns() const
{
    return compiledPatterns.keys();
}

void PatternEditor::addPatterns(const QStringList &patterns)
{
    if(!patterns.empty()){
        for(const QString& pattern: patterns){
           addPattern(pattern);
           update();
        }
        colorizeText();
        append("");
    }
    setFocus();
}

void PatternEditor::addPattern(const QString &pattern)
{
    if(!pattern.isEmpty()){
        QStringList fullPattern = pattern.split("=");
        if(fullPattern.length() > 1){
            QString name = fullPattern[0].simplified();
            fullPattern.pop_front();
            compiledPatterns[name] = fullPattern.join("=");
        } else if (fullPattern.length() == 1){
            compiledPatterns[fullPattern[0].simplified()] = "";
        }
    }
}

void PatternEditor::selectPattern(const QString &pattern)
{

}

QString PatternEditor::getText() const
{
    QStringList text = toPlainText().split("\n");
    QString result;
    for(QString s:text){
        if(!s.simplified().isEmpty()){
            result += s + '\n';
        }
    }
    return result;
}

void PatternEditor::keyPressEvent(QKeyEvent *e)
{
    if(isReadOnly()){
        if(toPlainText()[textCursor().position()].isNull()){
            qDebug() << e->key();
            if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return || e->key() == Qt::Key_Left || e->key() == Qt::Key_Right){
                qDebug() << "Попало";
                setTextColor(QColor("black"));
                setReadOnly(false);
            }
        }
    }
    QTextEdit::keyPressEvent(e);
}

void PatternEditor::colorizeText()
{
    QStringList text = toPlainText().split("\n");
    clear();
    QColor normal = textColor();
    for(QString s:text){
        QStringList listRepr = s.split("=");
        QString name = NOT_A_PATTERN;
        if(listRepr.length() > 0) name = listRepr[0].simplified();
        if(compiledPatterns.contains(name)){
            setTextColor( QColor( "green" ) );
        }else{
            setTextColor( QColor( "red" ) );
        }
        append(s);
    }
    setTextColor(normal);
}

QString PatternEditor::getPatternByLine(int num)
{
    QStringList text = toPlainText().split("\n");
    if(num >= text.length()){
        num = text.length() - 1;
    }
    if(!text.isEmpty() ){
        QStringList lst = text[num].split("=");
        if(lst.length() > 1) return lst[0];
    }
    return NOT_A_PATTERN;
}

int PatternEditor::getCursorLine()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    int lines = 0;
    while(cursor.positionInBlock()>0) {
        cursor.movePosition(QTextCursor::Up);
        lines++;
    }
    QTextBlock block = cursor.block().previous();

    while(block.isValid()) {
        lines += block.lineCount();
        block = block.previous();
    }
    return lines;
}

//void PatternEditor::paintEvent(QPaintEvent *_event)
//{
//    QPainter pnt(viewport());
//    for(QString pattern:compiledPatterns.keys()){
//        QString res = pattern + " = " +compiledPatterns[pattern];
//        int m = metrics->width(res);
//        int fontHeight = metrics->height() + 1;
//        int height = (m+6) / this->width() + 1;
//        int start = getPatternPos(pattern);
//        qDebug() << res;
//        qDebug() << start;
//        pnt.setPen( QColor( 0,255,0 ));
//        pnt.fillRect(QRectF(0,start*fontHeight,this->width(), height*fontHeight),QBrush(QColor(0,255,0)));
//    }
//    //
//    QTextEdit::paintEvent( _event );

//}

//int PatternEditor::getPatternPos(const QString &patternName)
//{

//    QRegExp regex("^[\\s\\t]*"+patternName +"[\\s\\t]*=[\\s\\t]*");
//    QStringList currentText = toPlainText().split("\n");
//    for(int i = 0;i<currentText.size();++i){
//        if(regex.indexIn(currentText[i])>=0){
//            return i;
//        }
//    }

//}

void PatternEditor::clearAll()
{
    clear();
    compiledPatterns.clear();
    patterns.clear();
}

void PatternEditor::checkEditable()
{
//    int pos = textCursor().position();
//    qDebug() << pos;
//    QString patName = getPatternByPos(pos);
//    qDebug() << patName;
//    if(compiledPatterns.contains(patName)){
//        setReadOnly(true);
//    }else{
//        setReadOnly(false);
//    }
    int currentLine = getCursorLine();
    QString patternName = getPatternByLine(currentLine);
    if(compiledPatterns.contains(patternName.simplified())){
        setReadOnly(true);
    }else{
        setReadOnly(false);
    }
}
