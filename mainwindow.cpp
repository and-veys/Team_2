#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "edit_window.h"
#include "mainmenu.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    mainEdit = new EditWindow(this);
    setCentralWidget(mainEdit);
    this->resize(500, 200);
    MainMenu * menu = new MainMenu(this, &textData);
    setMenuBar(menu);

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
    connect(menu, SIGNAL(createDocument()), this, SLOT(test()));
    connect(menu, SIGNAL(loadDocument()), this, SLOT(test()));
    connect(menu, SIGNAL(saveDocument(bool)), this, SLOT(test_2(bool)));
    connect(menu, SIGNAL(closeDocument()), this, SLOT(test()));
    connect(menu, SIGNAL(searchString()), this, SLOT(search_string_slot()));
    connect(menu, SIGNAL(searchImportance()), this, SLOT(search_importance_slot()));
    connect(menu, SIGNAL(setImportance(QString)), this, SLOT(test_3(QString)));
    connect(menu, SIGNAL(hideText(bool)), this, SLOT(test_2(bool)));
    connect(menu, SIGNAL(helpShow(QString)), this, SLOT(test_3(QString)));
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

}



