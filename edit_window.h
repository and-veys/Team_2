#ifndef EDITWINDOW_H
#define EDITWINDOW_H
#include <QPlainTextEdit>


class EditWindow: public QPlainTextEdit
{
    Q_OBJECT
public:
    EditWindow(const QString &text, QWidget *parent = nullptr);
    EditWindow(QWidget *parent = nullptr);
};

#endif // EDITWINDOW_H

