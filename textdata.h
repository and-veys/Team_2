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
    QString startTag;
    QTextCharFormat lineFormat;
};

class ParameterImportance : public ParametersTag {
public:
    ParameterImportance(QString name, QString tag, QColor col);
    ~ParameterImportance();
    QString nameImportance;
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
};

#endif // TEXTDATA_H
