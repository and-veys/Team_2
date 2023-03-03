#include "statusbar.h"
#include <QKeyEvent>
#include <QPlainTextEdit>
#include <QTime>
//#include "windows.h"          //АМВ: не кроссплатформенный подход !!

#include "textdata.h"

Team2StatusBar::Team2StatusBar(QWidget * par, TextData * dt):QStatusBar(par)
{
    auto createLabel = [this](int str=0){
        QLabel * lb = new QLabel(this);
        lb->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        lb->setAlignment(Qt::AlignCenter);
        lb->setMargin(5);
        addWidget(lb, str);
        return lb;
    };
    QLabel * mess = createLabel(10);
    QLabel * imp = createLabel(4);
    QLabel * bl = createLabel();
    QLabel * ch = createLabel();

    auto hd = dt->getParameterHide();
    namesImportance.insert(hd->getTag(), hd->getNameHide());
    auto im = dt->getSortListImportance();
    foreach(auto el, im) {
        namesImportance.insert(el->getTag(), el->getNameImportance());
    }
    connect(this, &Team2StatusBar::changeMessage, this,
            [mess](const QString & s){mess->setText("[" + QTime::currentTime().toString("hh:mm:ss") + "] " + s);});
    connect(this, &Team2StatusBar::changeCurrentBlock, this, [bl](const QString & s){bl->setText(s);});
    connect(this, &Team2StatusBar::changePositionInBlock, this, [ch](const QString & s){ch->setText(s);});
    connect(this, &Team2StatusBar::changeImportance, this, [imp](const QString & s){imp->setText(s);});
}
void Team2StatusBar::checkKeyEvent(QKeyEvent *event)
{
    //qDebug() << "KEY TEXT:" << event->text();
}

void Team2StatusBar::checkChangeCursorPosition()
{
    QPlainTextEdit * wnd = qobject_cast<QPlainTextEdit *>(sender());
    QTextCursor cursor = wnd->textCursor();
    auto getT = [](int n, const QString & f){
        QStringList lst = f.split(" ");
        lst[1] = QString::number(n).rightJustified(lst.at(1).length(), '0');
        return lst.join(" ");
    };
    QString tag = ParametersTag::getTag(cursor.charFormat());
    if(namesImportance.contains(tag)) tag = namesImportance.value(tag);
    else tag = "";
    emit this->changeCurrentBlock(getT(cursor.blockNumber()+1, "BLOCK: ###"));
    emit this->changePositionInBlock(getT(cursor.positionInBlock()+1, "POS: #####"));
    emit this->changeImportance(tag);
}
