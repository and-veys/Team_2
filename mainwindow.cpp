#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include "edit_window.h"
#include "mainmenu.h"

//#include "convertdata.h"

#include "dialogfind.h"
#include "dialoghelp.h"
#include "statusbar.h" // WND9-11


#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    mainEdit = new EditWindow(this);
    setCentralWidget(mainEdit);
    resize(700, 400);
//настройки фона окно, можно и вытащить потом отдельно в диалоговон окно
    QFont f = font();
    f.setPointSize(12);
    setFont(f);
//-----------------------------------------------------------



//-------------- инициализация диалоговых окон поиска

    dlgString = new DialogFindString("Поиск по строке", this);
    dlgHide = new DialogFindHide(textData.getParameterHide(), "Поиск по спрятанному", this);
    dlgImportance = new DialogFindImportance(textData.getSortListImportance(), "Поиск по важности", this);

    connect(dlgString, &DialogFind::search, this, &MainWindow::searchText);
    connect(dlgHide, &DialogFind::search, this, &MainWindow::searchHide);
    connect(dlgImportance, &DialogFind::search, this, &MainWindow::searchImportance);
//------------------------------------------------------------

//-------------- инициализация меню
    MainMenu * menu = new MainMenu(this, &textData);
    setMenuBar(menu);

    connect(menu, &MainMenu::searchString, this, [this](){dlgString->show();});
    connect(menu, &MainMenu::searchImportance, this, [this](){dlgImportance->show();});
    connect(menu, &MainMenu::searchHide, this, [this](){dlgHide->show();});



//------------------------------------------------------------


   //convertData = new ConvertData(this);       //АМВ: Заблокировано до выяснения обстоятельств
    fileFunction = new FileFunction(this);
   // mainEdit->setDisabled(true);//Гасим поле документа

    Team2StatusBar * stBar = new Team2StatusBar(this);
    setStatusBar(stBar);

    //signalAboutUs = new aboutus(this);

    fileFunction = new FileFunction(this);
    //mainEdit->setDisabled(true);//Гасим поле документа




//-------------------------------------

/*
    searchWidgetString.reset( new SearchWidgetString(QString("Поиск")));
    searchWidgetString->hide();
    searchWidgetImportance.reset(new SearchWidgetImportance(QString("Поиск"), textData));
    searchWidgetImportance->hide();

    connect(searchWidgetString.get(), &SearchWidgetString::searchPrev, mainEdit, &EditWindow::test_search_prev_slot);
    connect(searchWidgetString.get(), &SearchWidgetString::searchNext, mainEdit, &EditWindow::test_search_next_slot);
    connect(searchWidgetImportance.get(), &SearchWidgetString::searchPrev, mainEdit, &EditWindow::test_search_prev_slot);
    connect(searchWidgetImportance.get(), &SearchWidgetString::searchNext, mainEdit, &EditWindow::test_search_next_slot);

*/

//---------------------------------------------------
//Подставляйте свои receiver-объекты и их слоты
    /*Вихров*/    connect(menu, SIGNAL(createDocument()), this, SLOT(slotCreateDocument()));
    /*Вихров*/    connect(menu, SIGNAL(loadDocument()), fileFunction, SLOT(slotOpenFile()));
    /*Вихров*/    connect(fileFunction, SIGNAL(signalFileDataReady(QString*)), this, SLOT(slotRcvFileData(QString*)));//Слот для вывода прочитанного из файла текста
    /*Вихров*/    connect(menu, SIGNAL(saveDocument(bool)), this, SLOT(slotSaveDocument(bool)));//FALSE - сохранить под текущим именем,TRUE - сохранить как..
    /*Вихров*/    connect(this, SIGNAL(signalSaveDocument(QString*)),fileFunction, SLOT(slotSaveFile(QString *)));
    /*Вихров*/    connect(this, SIGNAL(signalSaveDocumentAs(QString*)),fileFunction, SLOT(slotSaveFileAs(QString *)));
    /*Вихров*/    connect(menu, SIGNAL(closeDocument()), this, SLOT(slotCloceDocument()));
    /*Вихров*/    connect(this, SIGNAL(signalCloseDocument(QString *)),fileFunction, SLOT(slotCloseFile(QString *)) );
//connect(menu, SIGNAL(signalTest()),this, SLOT(slotPrintDebug()) );


    connect(menu, SIGNAL(setImportance(QString)), this, SLOT(setImportance(QString)));  //установка важности

    connect(menu, SIGNAL(hideText(bool)), this, SLOT(hideText(bool)));                  //установка спратать/показать




//---------------------------------------------------
//    connect(mainEdit, SIGNAL(cursorPositionChanged()), stBar, SLOT(checkChangeCursorPosition()));
    connect(mainEdit, SIGNAL(cursorPositionChanged()), stBar, SLOT(checkChangeCursorPosition()));
//###    connect(this, SIGNAL(keyPressEvent(QKeyEvent *)), stBar, SLOT(checkKeyEvent(QKeyEvent *)));
    connect(menu, SIGNAL(helpShow(QString)), this, SLOT(helpShow(QString)));
    //connect(menu, SIGNAL(aboutUsShow(QString)), signalAboutUs, SLOT(slotAboutUs()));

//###    connect(menu, SIGNAL(helpShow(QString)), this, SLOT(test_3(QString)));
    connect(&textData, SIGNAL(errorSetFormat(QString)), this, SLOT(selectInformation(QString))); //информация о неуспешном выделении текста
    connect(mainEdit, SIGNAL(isForbiddenKey(QKeyEvent *)), &textData, SLOT(isForbiddenKey(QKeyEvent *))); //запрет клавиш и мест
    connect(mainEdit, SIGNAL(isHotKey(QKeyEvent *)), menu, SLOT(isHotKey(QKeyEvent *))); //горячие клавиши
//---------------------------------------------------
}



/*
//Тесты сигналов поиска
void MainWindow::search_string_slot() {
    searchWidgetString->show();
}
void MainWindow::search_importance_slot() {
    searchWidgetImportance->show();
}

*/
//---------------------------------------------------

MainWindow::~MainWindow()
{
    delete dlgString;
    delete dlgHide;
    delete dlgImportance;



    //fileFunction->~FileFunction();        //АМВ: Это как??
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
    //TODO Можно добавить в строку состояния или еще куда-нибудь
    QMessageBox::information(this, "You can`t do that", inf);
}

void MainWindow::helpShow(QString type)
{
    DialogHelp dlg(type, this);
    dlg.exec();
}

void MainWindow::searchText(DialogFind::searchEnum param, const QString & str)
{
    qDebug() << "TEXT" << ((param == DialogFind::NEXT) ? "next" : "prev") << str;

}

void MainWindow::searchImportance(DialogFind::searchEnum param, const QString &str)
{
    qDebug() << "IMP" << ((param == DialogFind::NEXT) ? "next" : "prev") << str;

}

void MainWindow::searchHide(DialogFind::searchEnum param, const QString &str)
{
    qDebug() << "HIDE" << ((param == DialogFind::NEXT) ? "next" : "prev") << str;
}




/*
Слот для вывода прочитанного из файла текста
Для работы создать:
connect(fileFunction, SIGNAL(signalFileDataReady(QString*)), this, SLOT(slotRcvFileData(QString*)));
*/
void MainWindow::slotRcvFileData(QString *text){
//АМВ Очень сложно - через сигналы, к тому же в объекте будет храниться первоначально загруженный текст файла!!

/* АМВ: Закоментировал, convertData->loadData решает только частный случай
   mainEdit->appendPlainText(*text);
   loadData(QString *gettingString, QPlainTextEdit *edtWin, TextData *textData)
   convertData->loadData(text, mainEdit, &textData);
*/
    textData.convertFromString(*text, mainEdit);
}

/**/
void MainWindow::slotSaveDocument(bool action){         //

/*  АМВ: Закоментировал, convertData->converterData решает только частный случай

    QString str = mainEdit->toPlainText();
    convertData->converterData(mainEdit,&textData, &str);
    qDebug() << "Try to save" << str;
*/


    QString str = textData.convertToString(mainEdit);
    if(action)
        emit signalSaveDocumentAs(&str);
    else
        emit signalSaveDocument(&str);
}

/*
Создаем новый документ
просто делаем поле активным
*/
void MainWindow::slotCreateDocument(){
    qDebug() << "CREATE !!";
    //mainEdit->setDisabled(false);
}

/*
Закрываем документ
*/
void MainWindow::slotCloceDocument(){
    QString str = mainEdit->toPlainText();
    emit signalCloseDocument(&str);
    mainEdit->clear();
    mainEdit->setEnabled(false);
}



/**/
void MainWindow::slotPrintDebug(){

    //convertData->converterData(mainEdit, &textData);
}

