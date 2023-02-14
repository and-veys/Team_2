#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textdata.h"
#include <QDebug>

class QPlainTextEdit;
class QPushButton;
class EditWindow;



class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    EditWindow* mainEdit;
    TextData textData;

private slots:
    //TODO ------------ Тесты функциональности, потом удалить
    void test() {qDebug() << "OK";};
    void test_2(bool a) {qDebug() << "OK:" << (a?"+":"-");};
    void test_3(QString a) {qDebug() << "TAG:" << a;};
    //---------------------------------------------------
};
#endif // MAINWINDOW_H
