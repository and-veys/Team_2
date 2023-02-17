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
//---------------------------------------------------
//Подставляйте свои receiver-объекты и их слоты
    connect(menu, SIGNAL(createDocument()), this, SLOT(test()));
    connect(menu, SIGNAL(loadDocument()), this, SLOT(test()));
    connect(menu, SIGNAL(saveDocument(bool)), this, SLOT(test_2(bool)));
    connect(menu, SIGNAL(closeDocument()), this, SLOT(test()));
    connect(menu, SIGNAL(searchString()), this, SLOT(test()));
    connect(menu, SIGNAL(searchImportance()), this, SLOT(test()));
    connect(menu, SIGNAL(setImportance(QString)), this, SLOT(setImportance(QString)));  //установка важности
    connect(menu, SIGNAL(hideText(bool)), this, SLOT(hideText(bool)));                  //установка спратать/показать
    connect(menu, SIGNAL(helpShow(QString)), this, SLOT(test_3(QString)));
    connect(&textData, SIGNAL(errorSetFormat(QString)), this, SLOT(selectInformation(QString))); //информация о неуспешном выделении текста
    connect(mainEdit, SIGNAL(isForbiddenKey(QKeyEvent *)), &textData, SLOT(isForbiddenKey(QKeyEvent *))); //запрет клавиш и мест
//---------------------------------------------------
// TODO удалить, это для теста---------------------------------------------------
    QString test_S = "Раз-Два-Три\nOne-Two-Three\nEin-Zwei-Drei";
   mainEdit->setPlainText(test_S);
//---------------------------------------------------
}

MainWindow::~MainWindow()
{

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

//TODO test, надо удалить
void MainWindow::test_3(QString a) {
    QTextCursor ch = mainEdit->textCursor();
    QString aa =ch.charFormat().property(1).toString();

    qDebug() << "TAG:" << aa << "BL:" <<ch.blockNumber();
    qDebug() << "BLP:" << ch.positionInBlock() << ":" << "POS:" << ch.position();


}



