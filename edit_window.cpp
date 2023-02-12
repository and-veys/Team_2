#include "edit_window.h"

EditWindow::EditWindow(const QString &text, QWidget *parent)
    : QPlainTextEdit(text, parent)
{

}

EditWindow::EditWindow(QWidget *parent) :
     QPlainTextEdit(parent)
{

}


