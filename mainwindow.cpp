#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "edit_window.h"
#include "mainmenu.h"
#include "statusbar.h" // WND9-11

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    mainEdit = new EditWindow(this);
    setCentralWidget(mainEdit);
    this->resize(500, 200);
    MainMenu * menu = new MainMenu(this, &textData);
    setMenuBar(menu);
    Team2StatusBar * stBar = new Team2StatusBar(this);
    setStatusBar(stBar);
    stBar->addLabels(stBar);

//---------------------------------------------------
//Подставляйте свои receiver-объекты и их слоты
    connect(menu, SIGNAL(createDocument()), this, SLOT(test()));
    connect(menu, SIGNAL(loadDocument()), this, SLOT(test()));
    connect(menu, SIGNAL(saveDocument(bool)), this, SLOT(test_2(bool)));
    connect(menu, SIGNAL(closeDocument()), this, SLOT(test()));
    connect(menu, SIGNAL(searchString()), this, SLOT(test()));
    connect(menu, SIGNAL(searchImportance()), this, SLOT(test()));
    connect(menu, SIGNAL(setImportance(QString)), this, SLOT(test_3(QString)));
    connect(menu, SIGNAL(hideText(bool)), stBar, SLOT(testSlotStatusBar(bool)));    // for test
    connect(menu, SIGNAL(unhideText(bool)), stBar, SLOT(testSlotStatusBar(bool)));  // for test
    connect(menu, SIGNAL(helpShow(QString)), this, SLOT(test_3(QString)));
    connect(menu, SIGNAL(aboutUsShow(QString)), this, SLOT(test_3(QString)));
    connect(mainEdit, SIGNAL(keyReleaseEvent(QKeyEvent *event)), stBar, SLOT(checkKeyEvent(QKeyEvent *event)));
//---------------------------------------------------
}

MainWindow::~MainWindow()
{

}



