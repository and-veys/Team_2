#ifndef TEXTDATA_H
#define TEXTDATA_H

#include <QColor>
#include <QMap>
#include <QObject>
#include <QTextCharFormat>


class ParametersTag {
public:
    ParametersTag(QString tag, QColor col);
    ~ParametersTag();
    QTextCharFormat lineFormat;
    QString getTag(){return lineFormat.property(1).toString();};
    QColor getColor(){return lineFormat.background().color();};
};

class ParameterImportance : public ParametersTag {
public:
    ParameterImportance(QString name, QString tag, QColor col);
    ~ParameterImportance();
    QString nameImportance;
    int id;
private:
    static int count;        //для сортировки по значимости, определяется порядком создания объекта
};
class ParameterHide : public ParametersTag {
public:
    ParameterHide(QString txt, QString tag, QColor col) ;
    ~ParameterHide();
    QString replacingText;
};
class TextData
{
public:
    TextData();
    ~TextData();
private:
    QMap<QString, QString> hidden_string;
    QMap<QString, ParameterImportance *> parametersImportance;
    ParameterHide * parametersHide;
public:

    ParameterImportance * getParameterImportance(const QString & key);
    ParameterHide * getParameterHide(){return parametersHide;};
    QList<ParameterImportance *> getSortListImportance();
};

#endif // TEXTDATA_H
