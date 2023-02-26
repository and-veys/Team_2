#include "edit_window.h"
#include <QDebug>
EditWindow::EditWindow(const QString &text, QWidget *parent)
    : QPlainTextEdit(text, parent)
{

}

EditWindow::EditWindow(QWidget *parent) :
     QPlainTextEdit(parent)
{

}

void EditWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event->key();
    bool hotKey = emit isHotKey(event);
    if(hotKey) return;                              //обработка горячих клавиш меню
    bool forbidden = emit isForbiddenKey(event);
    if(forbidden) return;                           //запрет на нажатие некоторых клавиш и мест вставки

    QPlainTextEdit::keyPressEvent(event);
}

void EditWindow::contextMenuEvent(QContextMenuEvent *event)
{
    //Убрал возможность контекстного меню, чтобы не было возможности paste и copy
    //можно потом и дописать обработку
    return;
    QPlainTextEdit::contextMenuEvent(event);
}




