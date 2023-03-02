#ifndef DIALOGFIND_H
#define DIALOGFIND_H

#include "textdata.h"

#include <QObject>
#include <QDialog>
#include <QDebug>
#include <QComboBox>

class DialogFind : public QDialog
{
    Q_OBJECT
public:
    enum searchEnum {
        NEXT = 0,
        PREV
    };
public:
    DialogFind(const QString & title, QWidget *parent = nullptr);
signals:
    void search(DialogFind::searchEnum, const QString &);

public slots:
    void next() {emit search(NEXT, searchData);};
    void prev() {emit search(PREV, searchData);};
    void closeDlg() {close();};
protected:
    QString searchData;
    void setMainWidget(QWidget * wnd);
};

class DialogFindString : public DialogFind {
    Q_OBJECT
public:
    DialogFindString(const QString & title, QWidget *parent = nullptr);
public slots:
    void setInformation(const QString & str) {searchData = str;};
};

class DialogFindHide : public DialogFind {
    Q_OBJECT
public:
    DialogFindHide(ParameterHide * hd, const QString & title, QWidget *parent = nullptr);
};


class DialogFindImportance : public DialogFind {
    Q_OBJECT
public:
    DialogFindImportance(QList<ParameterImportance *> imp, const QString & title, QWidget *parent = nullptr);
public slots:
    void setInformation(int) {searchData = combo->currentData().toString();};
private:
    QComboBox * combo;
};



#endif // DIALOGFIND_H
