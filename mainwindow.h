#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textdata.h"

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
};
#endif // MAINWINDOW_H
