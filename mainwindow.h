#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textdata.h"
#include <QDebug>

#include "search_widgets.h"
#include <memory>

#include "filefunction.h"
#include "convertdata.h"

//#include "aboutus.h"

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

    FileFunction *fileFunction;//Вихров
    //ConvertData *convertData;

    //aboutus *signalAboutUs;

private slots:
    void setImportance(QString tag);
    void hideText(bool hide);
    void selectInformation(QString inf);
    void helpShow(QString type);
    //TODO ------------ Тесты функциональности, потом удалить
    void test() {qDebug() << "OK";};
    void test_2(bool a) {qDebug() << "OK:" << (a?"+":"-");};
    void test_3(QString a){qDebug() << a;};
    //---------------------------------------------------

    void search_string_slot();
    void search_importance_slot();


    //Вихров
public slots:
    void slotRcvFileData(QString *text);
    void slotSaveDocument(bool action);
    void slotCreateDocument();
    void slotCloceDocument();
    /**/
    void slotPrintDebug();
    /**/

signals:
    void signalSaveDocument(QString *text);
    void signalSaveDocumentAs(QString *text);
    void signalCloseDocument(QString *text);

};
#endif // MAINWINDOW_H
