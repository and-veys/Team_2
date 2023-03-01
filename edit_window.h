#ifndef EDITWINDOW_H
#define EDITWINDOW_H
#include <QPlainTextEdit>
#include <QDebug>


class EditWindow: public QPlainTextEdit
{
    Q_OBJECT
public:
    EditWindow(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void focusInEvent(QFocusEvent *event);

signals:
    bool isForbiddenKey(QKeyEvent * event);
    bool isHotKey(QKeyEvent * event);
    void keyPressSignal(QKeyEvent * event);


public slots:



    //TODO ------------ Тесты функциональности сигналов от форм поиска, потом удалить
    void test_search_prev_slot(const QString& text){qDebug() << "Prev " + text;};
    void test_search_next_slot(const QString& text){qDebug() << "Next " + text;};
    //---------------------------------------------------



};

#endif // EDITWINDOW_H

