#include "topmenu.h"
#include <QDebug>
void TopMenu::setFileMenu()
{
    setFileActions();
    fileMenu = addMenu("Файл");
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveLsplAct);
    fileMenu->addAction(loadLsplAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
}

void TopMenu::setFileActions()
{
    newAct = new QAction(tr("Новый текстовый файл"),this);
    newAct->setShortcut(QKeySequence::New);
    newAct->setStatusTip(tr("Создает новый текстовый файл"));
    connect(newAct,SIGNAL(triggered()),this,SLOT(newFile()));

    openAct = new QAction(tr("Открыть текстовый файл"),this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Открывает существующий текстовый файл"));
    connect(openAct,SIGNAL(triggered()),this,SLOT(openFile()));

    saveAct = new QAction(tr("Сохранить текстовый файл"),this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохраняет текстовый файл"));
    connect(saveAct,SIGNAL(triggered()),this,SLOT(saveFile()));

    saveAsAct = new QAction(tr("Сохранить текстовый файл как"),this);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Сохранить как."));
    connect(saveAsAct,SIGNAL(triggered()),this,SLOT(saveFileAs()));

    saveLsplAct = new QAction(tr("Сохранить в LSPL формат"),this);
    saveLsplAct->setStatusTip(tr("Сохраняет шалоны, наложения и текст в lspl-формате."));
    connect(saveLsplAct,SIGNAL(triggered(bool)),this,SLOT(saveLspl()));

    loadLsplAct = new QAction(tr("Загрузить из LSPL формата"),this);
    loadLsplAct->setStatusTip(tr("Загружает файл из LSPL формата"));
    connect(loadLsplAct,SIGNAL(triggered(bool)),this,SLOT(loadLspl()));
}

void TopMenu::setPatternMenu()
{
    setPatternActions();
    patternsMenu = addMenu(tr("Шаблоны"));
    patternsMenu->addAction(importPatternsAct);
    patternsMenu->addAction(exportPatternsAct);
    patternsMenu->addSeparator();
    patternsMenu->addAction(clearPatternsAct);
}

void TopMenu::setPatternActions()
{

    importPatternsAct = new QAction(tr("Импортировать шаблоны"),this);
    importPatternsAct->setStatusTip(tr("Сохранить шаблоны в файл"));
    connect(importPatternsAct,SIGNAL(triggered()),this,SLOT(importPatterns()));

    exportPatternsAct = new QAction(tr("Экспортировать шаблоны"),this);
    exportPatternsAct->setStatusTip(tr("Загрузить шаблоны из файла"));
    connect(exportPatternsAct,SIGNAL(triggered()),this,SLOT(exportPatterns()));

    clearPatternsAct = new QAction(tr("Очистить шаблоны"),this);
    clearPatternsAct->setStatusTip(tr("Удалить все текущие шаблоны и их наложения"));
    connect(clearPatternsAct,SIGNAL(triggered(bool)),this,SLOT(clearPatterns()));

}

void TopMenu::setMatchesMenu()
{
    setMatchesAction();
    matchesMenu  = addMenu("Наложения");
    matchesMenu->addAction(exportMatchesAct);
}

void TopMenu::setMatchesAction()
{
    exportMatchesAct = new QAction(tr("Выгрузить наложения"),this);
    exportMatchesAct->setStatusTip(tr("Выгружает наложения в xml формате"));
    connect(exportMatchesAct,SIGNAL(triggered()),this,SLOT(exportMatches()));

}

void TopMenu::setSettingsMenu()
{
    setSettingsAction();
    settingsMenu = addMenu(tr("Настройки"));
    settingsMenu->addAction(setRMLAct);
}

void TopMenu::setSettingsAction()
{
    setRMLAct = new QAction(tr("Путь к RML"),this);
    setRMLAct->setStatusTip(tr("Указать путь к конфигурационному файлу морфологии"));
    connect(setRMLAct,&QAction::triggered,this,&TopMenu::setRml);
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
    text->saveTextFile(files.at(0));
}

void TopMenu::saveAllFiles()
{

}

void TopMenu::saveLspl()
{
    QString filter = "Xml Files (*.xml)";
    QString filePath = QFileDialog::getSaveFileName(this, tr("Сохранить в формате LSPL"),"", filter, &filter);
    if(!filePath.isEmpty()){
        if(!filePath.endsWith(".xml")){
            filePath = filePath.append(".xml");
        }
        cent->slotSaveLspl(filePath);
    }
}

void TopMenu::loadLspl()
{
    QString filter = "Xml Files (*.xml)";
    QString filePath = QFileDialog::getOpenFileName(this, tr("Загрузить из LSPL формата"),"", filter, &filter);
    if(!filePath.isEmpty()){
        cent->slotLoadLspl(filePath);
    }
}

void TopMenu::importPatterns()
{
    QString filter = "Pattern Files (*.pat)";
    QString filename = QFileDialog::getOpenFileName(this,"Выгрузить шаблоны","",filter,&filter);
    if(!filename.isEmpty()){
        pattern->importPatterns(filename);
    }
}

void TopMenu::exportPatterns()
{
    QString filter = "Pattern Files (*.pat)";
    QString filePath = QFileDialog::getSaveFileName(this, tr("Загрузить шаблоны"),"", filter, &filter);
    //qDebug() << filePath;
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
    QString filePath = QFileDialog::getSaveFileName(this, tr("Выгрузить наложения"),"", filter, &filter);
    //qDebug() << filePath;
    if(!filePath.isEmpty()){
        if(!filePath.endsWith(".xml")){
            filePath = filePath.append(".xml");
        }
       matches->saveMatches(filePath);
    }
}

void TopMenu::setRml()
{
    QFileDialog dialog(this);
    QString aotPath = QFileDialog::getExistingDirectory(this,tr("Путь к AOT"),QDir::currentPath(),QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if(!aotPath.isEmpty()){
#ifdef Q_OS_WIN
        QString path = "RML="+aotPath;
        putenv(path.toStdString().c_str());
#else
        setenv("RML",aotPath.toStdString().c_str(),1);
#endif
        comp->clear();
    }
}

TopMenu::TopMenu(QWidget* parent,CentralWidget* c):QMenuBar(parent),cent(c)
{
    text = c->getTextWidget();
    pattern = c->getPatternWidget();
    matches = c->getMatchesWidget();
    comp = c->getPatternCompiler();
    setDefaultUp(true);
    setNativeMenuBar(true);
    setFileMenu();
    setPatternMenu();
    setMatchesMenu();
    setSettingsMenu();

}

