#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textdata.h"
#include "dialogfind.h"
#include "mainfile.h"
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
    DialogFindString * dlgString;
    DialogFindHide * dlgHide;
    DialogFindImportance * dlgImportance;
    MainFile * mainFile;

    bool saveCurrentDocument();
    void createNewDocument();
    void setWindowsCaption(const QString & cap = "Новый документ");
    MainFile * createFile(bool save);

private slots:
    void setImportance(QString tag);
    void hideText(bool hide);
    void selectInformation(QString inf);
    void helpShow(QString type);
    void searchText(DialogFind::searchEnum param, const QString & str);
    void searchImportance(DialogFind::searchEnum param, const QString & str);
    void searchHide(DialogFind::searchEnum param, const QString & str);

    void slotCreateDocument();
    void slotOpenFile();
    void slotSaveDocument(bool as);
    void slotCloseWindow();
signals:
    void sendMessage(QString);
};
#endif // MAINWINDOW_H
