#include "topmenu.h"
#include <QDebug>
void TopMenu::setFileMenu()
{
    setFileActions();
    fileMenu = addMenu("File");
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveAllAct);
}

void TopMenu::setFileActions()
{
    newAct = new QAction(tr("&New"),this);
    newAct->setShortcut(QKeySequence::New);
    newAct->setStatusTip(tr("Create new file"));
    connect(newAct,SIGNAL(triggered()),this,SLOT(newFile()));

    openAct = new QAction(tr("&Open"),this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct,SIGNAL(triggered()),this,SLOT(openFile()));

    saveAct = new QAction(tr("&Save"),this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save file"));
    connect(saveAct,SIGNAL(triggered()),this,SLOT(saveFile()));

    saveAsAct = new QAction(tr("&Save as"),this);
    saveAsAct->setShortcut(QKeySequence::Save);
    saveAsAct->setStatusTip(tr("Save file as"));
    connect(saveAsAct,SIGNAL(triggered()),this,SLOT(saveFileAs()));

    saveAllAct = new QAction(tr("&Save all"),this);
    saveAllAct->setShortcut(QKeySequence::SelectAll);
    saveAllAct->setStatusTip(tr("Save all files"));
    connect(saveAllAct,SIGNAL(triggered()),this,SLOT(saveAllFiles()));

}

void TopMenu::setPatternMenu()
{
    setPatternActions();
    patternsMenu = addMenu("Patterns");
    patternsMenu->addAction(importPatternsAct);
    patternsMenu->addAction(exportPatternsAct);
    patternsMenu->addSeparator();
    patternsMenu->addAction(clearPatternsAct);
}

void TopMenu::setPatternActions()
{

    importPatternsAct = new QAction(tr("Import patterns"),this);
    importPatternsAct->setStatusTip(tr("Save patterns to file"));
    connect(importPatternsAct,SIGNAL(triggered()),this,SLOT(importPatterns()));

    exportPatternsAct = new QAction(tr("Export patterns"),this);
    exportPatternsAct->setStatusTip(tr("Load patterns from file"));
    connect(exportPatternsAct,SIGNAL(triggered()),this,SLOT(exportPatterns()));

    clearPatternsAct = new QAction(tr("Clear Patterns"),this);
    clearPatternsAct->setStatusTip(tr("Delete all patterns from namespace"));
    connect(clearPatternsAct,SIGNAL(triggered(bool)),this,SLOT(clearPatterns()));

}

void TopMenu::setMatchesMenu()
{
    setMatchesAction();
    matchesMenu  = addMenu("Matches");
    matchesMenu->addAction(exportMatchesAct);
}

void TopMenu::setMatchesAction()
{
    exportMatchesAct = new QAction(tr("Export Matches"),this);
    exportMatchesAct->setStatusTip(tr("Save matches to file"));
    connect(exportMatchesAct,SIGNAL(triggered()),this,SLOT(exportMatches()));

}

void TopMenu::newFile()
{
    text->newTextFile();
    filename = "";
}

void TopMenu::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if(!filename.isEmpty()){
        text->openTextFile(filename);
        this->filename = filename;
    }
}

void TopMenu::saveFile()
{
    QString fn = text->getCurrentFile();
    if(fn.isEmpty() || fn == "Untilted"){
        saveFileAs();
    }else{
        text->saveTextFile(fn);
    }
}

void TopMenu::saveFileAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec()) {
            files = dialog.selectedFiles();
    } else {
            return;
    }
    return text->saveTextFile(files.at(0));
}

void TopMenu::saveAllFiles()
{

}

void TopMenu::importPatterns()
{
    QString filter = "Pattern Files (*.pat)";
    QString filename = QFileDialog::getOpenFileName(this,"Import pattern","",filter,&filter);
    if(!filename.isEmpty()){
        //clearPatterns();
        pattern->importPatterns(filename);
    }
}

void TopMenu::exportPatterns()
{
    QFileDialog dialog(this);
    QString filter = "Pattern Files (*.pat)";
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export pattern"),"", filter, &filter);
    qDebug() << filePath;
    if(!filePath.isEmpty()){
        if(!filePath.endsWith(".pat")){
            filePath = filePath.append(".pat");
        }
        pattern->exportPatterns(filePath);
    }
}

void TopMenu::clearPatterns()
{
    text->slotClearMatches();
    pattern->slotClearPatterns();
}

void TopMenu::exportMatches()
{
    QString filter = "Matches Files (*.xml)";
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export matches"),"", filter, &filter);
    qDebug() << filePath;
    if(!filePath.isEmpty()){
        if(!filePath.endsWith(".xml")){
            filePath = filePath.append(".xml");
        }
       matches->saveMatches(filePath);
    }
}

TopMenu::TopMenu(QWidget* parent,CentralWidget* c):QMenuBar(parent),cent(c)
{
    text = c->getTextWidget();
    pattern = c->getPatternWidget();
    matches = c->getMatchesWidget();
    setDefaultUp(true);
    setNativeMenuBar(true);
    setFileMenu();
    setPatternMenu();
    setMatchesMenu();


}

