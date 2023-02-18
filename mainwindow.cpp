#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "edit_window.h"
#include "mainmenu.h"

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
connect(menu, SIGNAL(signalTest()),this, SLOT(slotPrintDebug()) );


    connect(menu, SIGNAL(setImportance(QString)), this, SLOT(setImportance(QString)));  //установка важности

    connect(menu, SIGNAL(hideText(bool)), this, SLOT(hideText(bool)));                  //установка спратать/показать


    connect(menu, SIGNAL(searchString()), this, SLOT(search_string_slot()));
    connect(menu, SIGNAL(searchImportance()), this, SLOT(search_importance_slot()));

    connect(menu, SIGNAL(helpShow(QString)), this, SLOT(test_3(QString)));
    connect(&textData, SIGNAL(errorSetFormat(QString)), this, SLOT(selectInformation(QString))); //информация о неуспешном выделении текста
    connect(mainEdit, SIGNAL(isForbiddenKey(QKeyEvent *)), &textData, SLOT(isForbiddenKey(QKeyEvent *))); //запрет клавиш и мест
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


void MainWindow::hideText(bool hide)
{
    if(hide)
        textData.hideText(mainEdit);
    else
        textData.showText(mainEdit);

}

void MainWindow::selectInformation(QString inf)
{
    //Можно добавить в строку состояния или еще куда-нибудь
   QMessageBox::information(this, "You can`t do that", inf);
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


/**/
void MainWindow::slotPrintDebug(){
qDebug() << "DEBUG";
QString str = mainEdit->toolTip();
qDebug() << str;
QTextCursor cursor;//Создаем экземпляр курсора
cursor = mainEdit->textCursor();//говорим, что этот экземпляр отосится к нашему текстовому окну
/********Хочу получить тэг скрытого текста**********/
int start = cursor.selectionStart();
int end = cursor.selectionEnd();
qDebug() << QString(tr("Start %1, End %2").arg(start).arg(cursor.selectionEnd()));
while(start <= end) {
    ParameterHide *parametr = textData.getParameterHide();
    qDebug() << parametr->getTag(cursor.charFormat()) << "marker* ";
//    qDebug() << parametr->getTag() << "marker - ";
    cursor.movePosition(QTextCursor::NextCharacter);
    ++start;
}


/*************************/
//    ParameterImportance *impotents;
//    QTextCharFormat ch = cursor.charFormat();
//    QString key;
//    textData.getParameterImportance( key);



//      cursor.charFormat();
// blockFormat
//QPlainTextEdit::textCursor() const;

//    QTextCharFormat textCharFormat = mainEdit->currentCharFormat();
//    textCharFormat.setForeground(Qt::darkGreen);
//    mainEdit->setCurrentCharFormat(textCharFormat);
//    mainEdit->setCurrentCharFormat(textCharFormat);
}
