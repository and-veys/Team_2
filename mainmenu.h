#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMenuBar>
#include <QObject>

class TextData;
class HotKey;

class MainMenu : public QMenuBar
{
    Q_OBJECT
public:
    MainMenu(QWidget * par, TextData * dt);
    ~MainMenu();
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
private:
    QMap<int, HotKey *> hotKey;
public slots:
    bool isHotKey(QKeyEvent * event);
};

class HotKey {
public:
    HotKey(QAction * act, Qt::KeyboardModifiers mod, int key);
    bool isHotKey(Qt::KeyboardModifiers mod);
private:
    QMap<Qt::KeyboardModifiers, QAction *> action;
    int key;
};


#endif // MAINMENU_H
