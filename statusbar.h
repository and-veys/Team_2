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
    void addLabels(QStatusBar * bar);
    void changeLabel(QLabel *lbl, QString str);

private slots:
    void testSlotStatusBar(bool item);
//    void checkKeyEvent(QKeyEvent *event);
};


#endif // STATUSBAR_H
