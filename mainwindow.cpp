#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolBar>
#include <QLabel>
#include <QMenu>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu* menuF = menuBar()->addMenu("Файл");
    menuF->addAction("Создать");
    menuF->addAction("Сохранить");
    menuF->addAction("Сохранить как...");
    menuF->addAction("Загрузить");
    menuF->addAction("Закрыть");
    QMenu* menuS = menuBar()->addMenu("Поиск");
    menuS->addAction("Поиск");
    QMenu* menuP = menuBar()->addMenu("Важность");
    menuP->addAction("Важность");
    QMenu* menuH = menuBar()->addMenu("Скрыть/показать");
    menuH->addAction("Скрыть");
    menuH->addAction("Показать");
    QMenu* menuR = menuBar()->addMenu("Справка");
    menuR->addAction("Справка");
    menuR->addAction("О разработчиках");
}

MainWindow::~MainWindow()
{
    delete ui;
}

