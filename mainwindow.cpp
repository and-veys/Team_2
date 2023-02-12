#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "edit_window.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    mainEdit = new EditWindow(this);
    setCentralWidget(mainEdit);


    /*
     * Пример добавления кнопок в меню бар, нужно будет удалить:
        menuBar()->addAction(QString("file"));
        menuBar()->addAction(QString("edit"));
    */

}

MainWindow::~MainWindow()
{

}

