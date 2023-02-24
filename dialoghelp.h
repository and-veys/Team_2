#ifndef DIALOGHELP_H
#define DIALOGHELP_H

#include <QDialog>
#include <QObject>

class DialogHelp : public QDialog
{
    Q_OBJECT
public:
    DialogHelp(QString & type, QWidget *parent = nullptr);
public slots:
    void closeDlg();
};

#endif // DIALOGHELP_H
