#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QLabel>

class Team2StatusBar : public QStatusBar
{
    Q_OBJECT
private:
    QLabel *labelColNum;
    QLabel *labelRowNum;
    QLabel *labelCapsLock;
    QLabel *labelInsert;
    QLabel *labelNumLock;
    QLabel *labelMessage;

public:
    Team2StatusBar(QWidget * par);
    void changeLabel(QLabel *lbl, QString str);

private slots:
    void testSlotStatusBar(bool item);
    void checkKeyEvent(QKeyEvent *event);
    void checkChangeCursorPosition();
    void showCursorPosition(int pos);
};


#endif // STATUSBAR_H
