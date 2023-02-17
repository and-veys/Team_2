#ifndef EDITWINDOW_H
#define EDITWINDOW_H
#include <QPlainTextEdit>


class EditWindow: public QPlainTextEdit
{
    Q_OBJECT
public:
    EditWindow(const QString &text, QWidget *parent = nullptr);
    EditWindow(QWidget *parent = nullptr);

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    bool isForbiddenKey(QKeyEvent * event);
};

#endif // EDITWINDOW_H

