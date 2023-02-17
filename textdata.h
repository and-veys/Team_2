#ifndef TEXTDATA_H
#define TEXTDATA_H

#include <QColor>
#include <QMap>
#include <QObject>
#include <QTextCharFormat>

class QPlainTextEdit;

class ParametersTag {
public:
    ParametersTag(QString tag, QColor col);
    ~ParametersTag();
    QString getTag(){return tag;};
    QString getTag(QTextCharFormat ch){return ch.property(1).toString();};
    QColor getColor(){return color;};
    void setParameters(QTextCharFormat & ch);

private:
    QColor color;
    QString tag;
};

class ParameterImportance : public ParametersTag {
public:
    ParameterImportance(QString name, QString tag, QColor col);
    ~ParameterImportance();
    QString getNameImportance(){return nameImportance;};
    int getId(){return id;};
private:
    QString nameImportance;
    int id;
    static int count;        //для сортировки по значимости, определяется порядком создания объекта
};
class ParameterHide : public ParametersTag {
public:
    ParameterHide(QString txt, QString tag, QColor col) ;
    ~ParameterHide();
    bool hasHideText(QTextCursor cursor);
private:
    QString replacingText;
};
class TextData : public QObject
{
    Q_OBJECT
public:
    TextData();
    TextData(const TextData& dt){};
    ~TextData();
private:
    QMap<QString, QString> hidden_string;               //массив для спрятанных строк
    QMap<QString, ParameterImportance *> parametersImportance;
    ParameterHide * parametersHide;
public:
    ParameterImportance * getParameterImportance(const QString & key);
    ParameterHide * getParameterHide(){return parametersHide;};
    QList<ParameterImportance *> getSortListImportance();
    void setImportance(QPlainTextEdit * wnd, QString & tag);
signals:
    void errorSetFormat(QString);
};

#endif // TEXTDATA_H
