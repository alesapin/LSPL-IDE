#include "topmenu.h"

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

TopMenu::TopMenu(QWidget* parent,CentralWidget* c):QMenuBar(parent),cent(c)
{
    setDefaultUp(true);
    setNativeMenuBar(true);
    setFileMenu();
    text = c->getTextWidget();

}

