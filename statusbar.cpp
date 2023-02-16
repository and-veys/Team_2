#include "statusbar.h"
#include "windows.h"

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
    labelMessage = new QLabel("Ready...");//сообщения, подсказки и т.п.
    labelMessage->setFrameStyle(QFrame::Panel | QFrame::Sunken);

}

void Team2StatusBar::addLabels(QStatusBar * bar)
{
    bar->addWidget(labelMessage,1);
    bar->addWidget(labelColNum);
    bar->addWidget(labelRowNum);
    bar->addPermanentWidget(labelCapsLock);
    bar->addPermanentWidget(labelInsert);
}

void Team2StatusBar::changeLabel(QLabel *lbl, QString str)
{
    lbl->setText(str);
}

void Team2StatusBar::testSlotStatusBar(bool item)
{
    changeLabel(labelMessage, item?"Hide Text":"UnhideText");
    if (GetKeyState(VK_CAPITAL) == 1)
        changeLabel(labelCapsLock, "CAPS");
    else
        changeLabel(labelCapsLock, "         ");
}

void Team2StatusBar::checkKeyEvent(QKeyEvent *event)
{
    if (GetKeyState(VK_CAPITAL) == 1)
        changeLabel(labelCapsLock, "CAPS");
    else
        changeLabel(labelCapsLock, "         ");

}
