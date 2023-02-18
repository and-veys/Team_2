#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textdata.h"
#include <QDebug>

#include "search_widgets.h"
#include <memory>

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
    std::unique_ptr<SearchWidgetString> searchWidgetString;
    std::unique_ptr<SearchWidgetImportance> searchWidgetImportance;

private slots:
    void setImportance(QString tag);
    void hideText(bool hide);
    void selectInformation(QString inf);
    //TODO ------------ Тесты функциональности, потом удалить
    void test() {qDebug() << "OK";};
    void test_2(bool a) {qDebug() << "OK:" << (a?"+":"-");};
    void test_3(QString a){qDebug() << "TAG:" << a;};
    //---------------------------------------------------

    void search_string_slot();
    void search_importance_slot();


};
#endif // MAINWINDOW_H
