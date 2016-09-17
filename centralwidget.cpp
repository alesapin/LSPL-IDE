#include "centralwidget.h"
#include <QDebug>

const int CentralWidget::DEFAULT_TIMEOUT = 30000;

const QString CentralWidget::CRASH_PATTERNS = "crash.pat";
const QString CentralWidget::CRASH_TEXT = "crash.txt";

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent), timeOut(DEFAULT_TIMEOUT)
{

    watcher = new QFutureWatcher<QSharedPointer<PatternViewMap>>();
    compiler = new PatternCompiler();
    txt = new TextBasicWidget(this);
    pattern = new PatternsBasicWidget(compiler,this);
    matches = new MatchesBasicWidget(this);
    timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    QSplitter* vert = new QSplitter();
    QSplitter* horiz = new QSplitter();
    QWidget* containter = new QWidget();
    QHBoxLayout* containterLay = new QHBoxLayout();
    QVBoxLayout* mainLay = new QVBoxLayout();
    containterLay->setContentsMargins(0,0,0,0);
    horiz->addWidget(txt);
    horiz->addWidget(pattern);
    containterLay->addWidget(horiz);
    containter->setLayout(containterLay);
    vert->setOrientation(Qt::Vertical);
    vert->addWidget(containter);
    vert->addWidget(matches);

//    vert->addWidget(progress);
    mainLay->addWidget(vert);
    setLayout(mainLay);
    connect(pattern,SIGNAL(matchClicked()), this,SLOT(slotAnalyze()));
    connect(matches,SIGNAL(patternWasUnchecked(QString)),txt,SLOT(slotPatternUncheked(QString)));
    connect(matches,SIGNAL(patternWasChecked(QString)),txt,SLOT(slotPatternChecked(QString)));
    connect(matches,SIGNAL(showAll()),txt,SLOT(slotHighlightAll()));
    connect(matches,SIGNAL(showAll()),pattern,SLOT(slotDisableMatch()));
    connect(matches,SIGNAL(hideAll()),txt,SLOT(slotDehighlightAll()));
    connect(matches,SIGNAL(rowClicked(int,int)),txt,SLOT(slotSelectFragment(int,int)));
    connect(matches,SIGNAL(patternWasUnchecked(QString)),this,SIGNAL(statusHighlighting()));
    connect(matches,SIGNAL(patternWasChecked(QString)),this,SIGNAL(statusHighlighting()));

    connect(txt,SIGNAL(tabChanged(int)),matches,SLOT(slotChangeTab(int)));
    connect(txt,SIGNAL(tabClosed(int)),matches,SLOT(slotCloseTab(int)));
    connect(txt,SIGNAL(editEnabled()),matches,SLOT(slotClear()));
    connect(txt,SIGNAL(editEnabled()),this,SLOT(slotEdit()));
    connect(txt,SIGNAL(checkingEnabled()),matches,SLOT(slotEnableChecking()));
    connect(txt,SIGNAL(checkingEnabled()),this,SIGNAL(statusReady()));
    connect(txt, SIGNAL(checkingEnabled()),pattern, SLOT(slotEnableMatch()));

    connect(watcher,SIGNAL(finished()),this,SLOT(slotDisplay()));
    connect(timeoutTimer,SIGNAL(timeout()),this,SLOT(slotTimeout()));

    //connect(watcher,SIGNAL(progressValueChanged(int)),this,SLOT(slotProgress(int)));
    if (QFile::exists(CRASH_TEXT) && QFile::exists(CRASH_PATTERNS)){
        loadAfterCrash(CRASH_TEXT, CRASH_PATTERNS);
    }
}

CentralWidget::~CentralWidget()
{
    delete compiler;
}

TextBasicWidget *CentralWidget::getTextWidget()
{
    return txt;
}

PatternsBasicWidget *CentralWidget::getPatternWidget()
{
    return pattern;
}

MatchesBasicWidget *CentralWidget::getMatchesWidget()
{
    return matches;
}

PatternCompiler *CentralWidget::getPatternCompiler()
{
    return compiler;
}

QStringList CentralWidget::getChoosenPatterns()
{
    return pattern->getChoosenPatternsNames();
}

void CentralWidget::loadAfterCrash(const QString &textFileName, const QString &patternFileName)
{
    txt->openTextFile(textFileName);
    pattern->importPatterns(patternFileName);
    QFile::remove(textFileName);
    QFile::remove(patternFileName);
}

void CentralWidget::createBackup() const
{
    QFile text(CRASH_TEXT);
    if(text.open(QFile::WriteOnly | QFile::Text)){
        QTextStream ts(&text);
        ts << txt->getText();
    }
    text.close();
    QFile pats(CRASH_PATTERNS);
    if(pats.open(QFile::WriteOnly | QFile::Text)){
        QTextStream ps(&pats);
        ps << pattern->getPatternsAsText();
    }
    pats.close();
}

void CentralWidget::slotAnalyze()
{
    QStringList patternNames = pattern->getChoosenPatternsNames();
    if(!patternNames.isEmpty() && !txt->getText().isEmpty()){
        emit statusEngine();
        txt->setReadOnly(true);
        QFuture<QSharedPointer<PatternViewMap>> future = QtConcurrent::run(compiler,&PatternCompiler::analyzeText,patternNames,txt->getText());
        watcher->setFuture(future);
        timeoutTimer->start(timeOut);
    } else {
        pattern->slotEnableMatch();
    }
}

void CentralWidget::slotDisplay()
{
    if(!watcher->future().isCanceled()){
        timeoutTimer->stop();
        QStringList patternNames = pattern->getChoosenPatternsNames();
        QSharedPointer<PatternViewMap> m = watcher->future().result();
        QSharedPointer<utility::IntervalViewMap> convertedResult = utility::convertMatchesToIntervals(*m);
        if(!convertedResult->empty() && !txt->getText().isEmpty()){
             emit statusHighlighting();
             txt->setMatches(convertedResult);
             matches->setMatches(convertedResult,patternNames);
        }else if(convertedResult->empty()){
            matches->setMatches(convertedResult,patternNames);
        }
    }
}

void CentralWidget::slotEdit()
{
    watcher->cancel();
    emit statusCanceled();
}
/**
 * TODO
 * @brief CentralWidget::slotTimeout
 * Ужасный метод, вызываемый из-за невозможности убить
 * поток в Qt. Переделать!!!
 */
void CentralWidget::slotTimeout()
{
    QMessageBox msgBox(
                QMessageBox::Information,
                tr("Нет ответа"),
                tr("<p align='center'>Наложение шаблонов задумалось... \
                   Возможно Вы написали неправильные шаблоны, или \
                   предложенный Вами текст слишком большой. Проверьте \
                   код более тщательно. Рекомендуется перечитать документацию \
                   шаблонов и перезагрузить приложение."),
                   QMessageBox::Yes | QMessageBox::No);

    msgBox.setButtonText(QMessageBox::Yes, tr("Reboot app"));
    msgBox.setButtonText(QMessageBox::No, tr("Wait"));

    if (msgBox.exec() == QMessageBox::Yes) {
        createBackup();
        watcher->thread()->sleep(1);
        QProcess::startDetached(QApplication::applicationFilePath());
        int pid = QCoreApplication::applicationPid();
#ifndef Q_OS_WIN
        system(QString("kill " + QString::number(pid)).toStdString().c_str());
#endif
        QCoreApplication::quit();
    }
}

void CentralWidget::slotProgress(int val)
{
    //qDebug() << "Progress Value:" << val;
}

void CentralWidget::slotSaveLspl(const QString &filename)
{
    xml.setMatches(matches->getSelectedMatches());
    xml.setPatterns(pattern->getPatterns());
    xml.setText(txt->getText());
    xml.writeFile(filename);
}

void CentralWidget::slotLoadLspl(const QString &filename)
{
    if(xml.readFile(filename)){
        pattern->loadPatterns(xml.getPatterns());
        txt->addText(xml.getText());
        QSharedPointer<utility::IntervalViewMap> mths = xml.getMatches();
        emit statusHighlighting();
        txt->setMatches(mths);
        matches->setMatches(mths);
    }
}

