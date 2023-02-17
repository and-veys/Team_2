#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include "textdata.h"
class QLayout;
class QPushButton;
class QAbstractScrollArea;


class AbstractSearchWidget: public QWidget
{
    Q_OBJECT
    QString label;
    virtual void create_data_widget() = 0;

protected:
    QWidget* data_widget;
    QPushButton * prev;
    QPushButton * next;
    QString searcheble_text = "";
    QLayout* layout;
public:
    AbstractSearchWidget(const QString& label=QString(), QWidget * parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

signals:
    void searchPrev(const QString& str);
    void searchNext(const QString& str);
};




class SearchWidgetImportance: public AbstractSearchWidget {

    Q_OBJECT
    void create_data_widget() override;
    TextData td;

public:
    SearchWidgetImportance(
            const QString& label,
            const TextData& td,
            QWidget * parent = nullptr,
            Qt::WindowFlags f = Qt::WindowFlags());
public slots:
    void AMV_FUNC_1(int ind);

};



class SearchWidgetString: public AbstractSearchWidget {

    void create_data_widget() override;

public:
    SearchWidgetString(const QString& label=QString(), QWidget * parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

};

#endif // SEARCHWIDGET_H
