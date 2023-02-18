#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "edit_window.h"
#include "mainmenu.h"


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
    MainMenu * menu = new MainMenu(this, &textData);
    setMenuBar(menu);

    fileFunction = new FileFunction(this);
    mainEdit->setDisabled(true);//Гасим поле документа

    searchWidgetString.reset( new SearchWidgetString(QString("Поиск")));
    searchWidgetString->hide();
    searchWidgetImportance.reset(new SearchWidgetImportance(QString("Поиск"), textData));
    searchWidgetImportance->hide();

    connect(searchWidgetString.get(), &SearchWidgetString::searchPrev, mainEdit, &EditWindow::test_search_prev_slot);
    connect(searchWidgetString.get(), &SearchWidgetString::searchNext, mainEdit, &EditWindow::test_search_next_slot);
    connect(searchWidgetImportance.get(), &SearchWidgetString::searchPrev, mainEdit, &EditWindow::test_search_prev_slot);
    connect(searchWidgetImportance.get(), &SearchWidgetString::searchNext, mainEdit, &EditWindow::test_search_next_slot);
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

    connect(menu, SIGNAL(setImportance(QString)), this, SLOT(setImportance(QString)));  //установка важности

    connect(menu, SIGNAL(searchString()), this, SLOT(search_string_slot()));
    connect(menu, SIGNAL(searchImportance()), this, SLOT(search_importance_slot()));

    connect(menu, SIGNAL(hideText(bool)), this, SLOT(test_2(bool)));
    connect(menu, SIGNAL(helpShow(QString)), this, SLOT(test_3(QString)));
//---------------------------------------------------


// TODO удалить, это для теста---------------------------------------------------
    QString test_S = "Раз-Два-Три\nOne-Two-Three\nEin-Zwei-Drei";
    mainEdit->setPlainText(test_S);
//---------------------------------------------------
}

//Тесты сигналов поиска
void MainWindow::search_string_slot() {
    searchWidgetString->show();
}
void MainWindow::search_importance_slot() {
    searchWidgetImportance->show();
}
//---------------------------------------------------

MainWindow::~MainWindow()
{
    fileFunction->~FileFunction();
}

void MainWindow::setImportance(QString tag)
{
    textData.setImportance(mainEdit, tag);
}



/*
Слот для вывода прочитанного из файла текста
Для работы создать:
connect(fileFunction, SIGNAL(signalFileDataReady(QString*)), this, SLOT(slotRcvFileData(QString*)));
*/
void MainWindow::slotRcvFileData(QString *text){
    mainEdit->appendPlainText(*text);
}

/**/
void MainWindow::slotSaveDocument(bool action){
    QString str = mainEdit->toPlainText();
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
    mainEdit->setDisabled(false);
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
