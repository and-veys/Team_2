#include "mainwindow.h"
#include "edit_window.h"
#include "mainmenu.h"
#include "dialogfind.h"
#include "dialoghelp.h"
#include "statusbar.h" // WND9-11
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
//-----------------------------------------------------------
//-------------- настройки главного окна
// TODO можно вытащить потом отдельно в диалоговое окно пользовательских настроек


    setWindowTitle("Team #2: Упс");
    resize(1000, 400);
    connect(&textData, &TextData::errorSetFormat, this, &MainWindow::selectInformation);
//------------------------------------------------------------
//-------------- инициализация меню

    MainMenu * menu = new MainMenu(this, &textData);
    setMenuBar(menu);
    connect(menu, &MainMenu::createDocument, this, &MainWindow::slotCreateDocument);
    connect(menu, &MainMenu::loadDocument, this, &MainWindow::slotOpenFile);
    connect(menu, &MainMenu::saveDocument, this, &MainWindow::slotSaveDocument);
    connect(menu, &MainMenu::closeDocument, this, &MainWindow::slotCloseWindow);
    connect(menu, &MainMenu::setImportance, this, &MainWindow::setImportance);  //установка важности
    connect(menu, &MainMenu::hideText, this, &MainWindow::hideText);            //установка спратать/показать
    connect(menu, &MainMenu::searchString, this, [this](){dlgString->show();});
    connect(menu, &MainMenu::searchImportance, this, [this](){dlgImportance->show();});
    connect(menu, &MainMenu::searchHide, this, [this](){dlgHide->show();});
    connect(menu, &MainMenu::helpShow, this, &MainWindow::helpShow);

//-----------------------------------------------------------
//-------------- инициализация строки состояния

    Team2StatusBar * stBar = new Team2StatusBar(this, &textData);
    setStatusBar(stBar);    
    connect(&textData, &TextData::errorSetFormat, this, [stBar](QString s){emit stBar->changeMessage(s);});
    connect(this, &MainWindow::sendMessage, this, [stBar](QString s){emit stBar->changeMessage(s);});

//------------------------------------------------------------
//-------------- инициализация виджета ввода текста

    mainEdit = new EditWindow(this);
    setCentralWidget(mainEdit);
    connect(mainEdit, SIGNAL(isForbiddenKey(QKeyEvent *)), &textData, SLOT(isForbiddenKey(QKeyEvent *)));   //запрет клавиш и мест
    connect(mainEdit, SIGNAL(isHotKey(QKeyEvent *)), menu, SLOT(isHotKey(QKeyEvent *)));                    //горячие клавиши
    connect(mainEdit, &EditWindow::cursorPositionChanged, stBar, &Team2StatusBar::checkChangeCursorPosition);
    connect(mainEdit, &EditWindow::keyPressSignal, stBar, &Team2StatusBar::checkKeyEvent);

//-----------------------------------------------------------
//-------------- инициализация диалоговых окон поиска

    dlgString = new DialogFindString("Поиск по строке", this);
    dlgHide = new DialogFindHide(textData.getParameterHide(), "Поиск по спрятанному", this);
    dlgImportance = new DialogFindImportance(textData.getSortListImportance(), "Поиск по важности", this);
    connect(dlgString, &DialogFind::search, this, &MainWindow::searchText);
    connect(dlgHide, &DialogFind::search, this, &MainWindow::searchHide);
    connect(dlgImportance, &DialogFind::search, this, &MainWindow::searchImportance);
//------------------------------------------------------------
//-------------- инициализация объекта работы с файлом
    mainFile = nullptr;
    createNewDocument();


//------------------------------------------------------------

}

MainWindow::~MainWindow()
{
    delete dlgString;
    delete dlgHide;
    delete dlgImportance;
    if(mainFile) delete mainFile;
}

void MainWindow::setImportance(QString tag)
{
   textData.setImportance(mainEdit, tag);
}


void MainWindow::hideText(bool hide)
{
    if(hide)
        textData.hideText(mainEdit);
    else
        textData.showText(mainEdit);

}

void MainWindow::selectInformation(QString inf)
{
    QMessageBox::information(this, "You can`t do that", inf);
}

void MainWindow::helpShow(QString type)
{
    DialogHelp dlg(type, this);
    dlg.exec();
}

void MainWindow::searchText(DialogFind::searchEnum param, const QString & str)      //TODO
{
    qDebug() << "TEXT" << ((param == DialogFind::NEXT) ? "next" : "prev") << str;

}

void MainWindow::searchImportance(DialogFind::searchEnum param, const QString &str) //TODO
{
    qDebug() << "IMP" << ((param == DialogFind::NEXT) ? "next" : "prev") << str;

}

void MainWindow::searchHide(DialogFind::searchEnum param, const QString &str)   //TODO
{
    qDebug() << "HIDE" << ((param == DialogFind::NEXT) ? "next" : "prev") << str;
}

bool MainWindow::saveCurrentDocument()
{
   int res = QMessageBox::question(this, "Вопрос от приложения", "Сохранить текущий документ?",
                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
   if(res == QMessageBox::Yes) slotSaveDocument(false);
   if(res != QMessageBox::Cancel) return true;
   emit sendMessage("Операция отменена");
   return false;
}

void MainWindow::createNewDocument()
{
    if(mainFile)
        delete mainFile;
    textData.clear();
    mainFile = nullptr;
    textData.convertFromString(textData.getNormalText()->getTag()+"A", mainEdit);
    mainEdit->clear();
    setWindowsCaption();
}
void MainWindow::setWindowsCaption(const QString &cap)
{
    QStringList lst = windowTitle().split(": ");
    lst[lst.length() - 1] = cap;
    setWindowTitle(lst.join(": "));
}

void MainWindow::slotCreateDocument(){
    if(saveCurrentDocument())
        createNewDocument();
}
MainFile * MainWindow::createFile(bool save) {
    MainFile *fl = new MainFile();
    connect(fl, &MainFile::sendMessage, this, [this](QString s){emit sendMessage(s);});
    connect(fl, &MainFile::sendWindowCaption, this, [this](QString s){setWindowsCaption(s);});
    if(fl->create(this, save)) return fl;
    delete fl;
    return nullptr;
}
void MainWindow::slotSaveDocument(bool as){
    if(as || mainFile == nullptr) {
        MainFile * fl = createFile(true);
        if(fl) {
           if(mainFile) delete mainFile;
           mainFile = fl;
        }
        else return;
    }
    QString str = textData.convertToString(mainEdit);
    mainFile->save(str);
}

void MainWindow::slotOpenFile() {
    if(saveCurrentDocument()) {
        createNewDocument();
        MainFile * fl = createFile(false);
        if(fl) {
           mainFile = fl;
           QString res;
           if(mainFile->load(res))
                textData.convertFromString(res, mainEdit);
        }
    }
}

void MainWindow::slotCloseWindow(){
    if(saveCurrentDocument())
        close();
}


