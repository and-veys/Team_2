#ifndef TEXTDATA_H
#define TEXTDATA_H

#include <QColor>
#include <QMap>
#include <QObject>

class ParameterImportance {
public:
    ParameterImportance(QString name, QString tag, QColor col);
    QString nameImportance;
    QString startTag;
    QString endTag;
    QColor color;
};
class ParameterHide {
public:
    ParameterHide(QString txt, QString tag  = "hide");
    QString replacingText;
    QString startTag;
    QString endTag;
};


class TextData
{
public:
    TextData();
    ~TextData();
private:
    QString data;
    QMap<QString, ParameterImportance *> parametersImportance;
    ParameterHide * parametersHide;
public:
    ParameterImportance * getParameterImportance(const QString & key){
        return (parametersImportance.contains(key) ? parametersImportance.take(key) : nullptr);
    };
    ParameterHide * getParameterHide(){return parametersHide;};
};

#endif // TEXTDATA_H
