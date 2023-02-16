#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMenuBar>
#include <QObject>

class TextData;

class MainMenu : public QMenuBar
{
    Q_OBJECT
public:
    MainMenu(QWidget * par, TextData * dt);
signals:
    void createDocument();
    void loadDocument();
    void saveDocument(bool);
    void closeDocument();
    void searchString();
    void searchImportance();
    void setImportance(QString);
    void hideText(bool);
    void unhideText(bool);
    void helpShow(QString);
    void aboutUsShow(QString);
};

#endif // MAINMENU_H
