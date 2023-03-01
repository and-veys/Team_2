#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QLabel>
#include <QTextCursor>

class TextData;

class Team2StatusBar : public QStatusBar
{
    Q_OBJECT
private:
    QMap<QString, QString> namesImportance;
public:
    Team2StatusBar(QWidget * par, TextData * dt);
public slots:
    void checkKeyEvent(QKeyEvent *event);
    void checkChangeCursorPosition();
signals:
    void changeMessage(const QString &);
    void changeCurrentBlock(const QString &);
    void changePositionInBlock(const QString &);
    void changeImportance(const QString &);
};


#endif // STATUSBAR_H
