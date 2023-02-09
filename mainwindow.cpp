#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
    MainEdit = new QPlainTextEdit(this);
    setCentralWidget(MainEdit);

    /*
     * Пример добавления кнопок в меню бар, нужно будет удалить:
        menuBar()->addAction(QString("file"));
        menuBar()->addAction(QString("edit"));
    */
}

MainWindow::~MainWindow()
{
//    delete ui;
}

