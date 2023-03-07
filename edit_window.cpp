#include "edit_window.h"
#include <QDebug>

EditWindow::EditWindow(QWidget *parent):QPlainTextEdit(parent){
    QFont f = font();
    f.setPointSize(12);
    setFont(f);
    setCursorWidth(3);
    setAcceptDrops(false);

    QPalette p = palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
    setPalette(p);
}

void EditWindow::keyPressEvent(QKeyEvent *event)
{

    emit keyPressSignal(event);
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

void EditWindow::focusInEvent(QFocusEvent *event)
{

    emit cursorPositionChanged();
    QPlainTextEdit::focusInEvent(event);
}




