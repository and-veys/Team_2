#include "statusbar.h"
#include <QKeyEvent>
#include "windows.h"
#include "mainwindow.h"


Team2StatusBar::Team2StatusBar(QWidget * par):QStatusBar(par)
{
    labelColNum = new QLabel("Col:    "); //позиция курсора в строке
    labelColNum->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    labelRowNum = new QLabel("Row:    "); //строка, на которой курсор
    labelRowNum->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    labelCapsLock = new QLabel("CAPS");   //индикация нажатой клавиши "CapsLock"
    labelCapsLock->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    labelInsert = new QLabel("INS");      //индикация режима вставки/замены
    labelInsert->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    labelNumLock = new QLabel("NUM");      //индикация режима вставки/замены
    labelNumLock->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    labelMessage = new QLabel("Ready...");//сообщения, подсказки и т.п.
    labelMessage->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    this->addWidget(labelMessage,1);
    this->addWidget(labelColNum);
    this->addWidget(labelRowNum);
    this->addPermanentWidget(labelCapsLock);
    this->addPermanentWidget(labelInsert);
    this->addPermanentWidget(labelNumLock);

}

void Team2StatusBar::changeLabel(QLabel *lbl, QString str)
{
    lbl->setText(str);
}

void Team2StatusBar::checkKeyEvent(QKeyEvent *event)
{
    if (event->key() == VK_CAPITAL)
    {
        if (GetKeyState(VK_CAPITAL) == 1)
        {
            changeLabel(labelCapsLock, "CAPS");
        }else{
            changeLabel(labelCapsLock, "         ");
        }
    }
}

void Team2StatusBar::checkChangeCursorPosition()
{
    changeLabel(labelMessage, "Cursor was change position");

}

void Team2StatusBar::showCursorPosition(int pos)
{
    changeLabel(labelColNum, QString::number(pos));
}
/*

void MainWindow::checkChangeCursorPosition()
{
     Team2StatusBar::showCursorPosition(mainEdit->textCursor().position());
}
*/

// only for testing, must be delete
void Team2StatusBar::testSlotStatusBar(bool item)
{
    changeLabel(labelMessage, item?"Hide Text":"UnhideText");

    if (GetKeyState(VK_CAPITAL) == 1)
        changeLabel(labelCapsLock, "CAPS");
    else
        changeLabel(labelCapsLock, "         ");
    if (GetKeyState(VK_INSERT) == 1)
        changeLabel(labelInsert, "INS");
    else
        changeLabel(labelInsert, "         ");
    if (GetKeyState(VK_NUMLOCK) == 1)
        changeLabel(labelNumLock, "NUM");
    else
        changeLabel(labelNumLock, "         ");

//    mainEdit::cursorForPosition()



}

/*
------------ from mainwindow.cpp
    connect(mainEdit, SIGNAL(keyReleaseEvent(QKeyEvent *event)), stBar, SLOT(checkKeyEvent(QKeyEvent *event)));
------------
void Team2StatusBar::checkKeyEvent(QKeyEvent *event)
{
    if (GetKeyState(VK_CAPITAL) == 1)
        changeLabel(labelCapsLock, "CAPS");
    else
        changeLabel(labelCapsLock, "         ");

}
*/
