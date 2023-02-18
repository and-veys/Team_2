#ifndef TEXTDATA_H
#define TEXTDATA_H

#include <QColor>
#include <QMap>
#include <QObject>
#include <QTextCharFormat>
#include <QTextCursor>

class QPlainTextEdit;
class QKeyEvent;
class ParameterHide;
class ParameterImportance;

class TextData : public QObject
{
    Q_OBJECT
public:
    TextData();
    TextData(const TextData&){};
    ~TextData();

    enum errorEnum {
        NOT_SELECT = 0,
        HIDE_SELECT,
        SOME_SELECT,
        NOT_HIDE,
        NOT_NORMAL
    };
    enum placeEnum {
        NOT = 1,
        START = 2,
        CENTER = 4,
        END = 8,
        NORMAL = 128
    };
private:
    QMap<int, QString> hiddenString;               //массив для спрятанных строк
    QMap<QString, ParameterImportance *> parametersImportance;
    QMap<errorEnum, QString> errorTexts;
    ParameterHide * parametersHide;
    void sendErrorSignal(errorEnum key);
    ParameterImportance * getNormalText(){return getParameterImportance("!");};
public:
    ParameterImportance * getParameterImportance(const QString &key);
    ParameterHide * getParameterHide(){return parametersHide;};
    QList<ParameterImportance *> getSortListImportance();
    void setImportance(QPlainTextEdit * wnd, QString & tag);
    void hideText(QPlainTextEdit * wnd);
    void showText(QPlainTextEdit * wnd);
signals:
    void errorSetFormat(QString);
public slots:
    bool isForbiddenKey(QKeyEvent * event);
};


class ParametersTag {
public:
    ParametersTag(QString tag, QColor col);
    ~ParametersTag();
    QString getTag(){return tag;};
    QString getTag(QTextCharFormat ch){return ch.property(1).toString();};
    QColor getColor(){return color;};
    void setParameters(QTextCharFormat & ch);
    bool hasCharsFormat(QTextCursor cursor){return isCharFormat(cursor, false);};       //хоть один символ формата объекта
    bool allCharsFormat(QTextCursor cursor){return isCharFormat(cursor, true);};        //все символы формата объекта
    bool findCharsFormat(QTextCursor& cursor);                  //выделение строки текущего формат
    int findLimitFormat(QTextCursor cursor, bool next);

private:
    QColor color;
    QString tag;
    bool isCharFormat(QTextCursor & cursor, bool all);
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
    QString getReplacingText(){return replacingText;};
    int setParameters(QTextCharFormat & ch);
    int getPlaceCursor(QTextCursor cursor, QTextCharFormat & ch);
    int getHideKey(QTextCursor & cursor){return cursor.charFormat().property(2).toInt();};
private:
    QString replacingText;
    static int id;        //для уникального идентификатора спрятанного текста

};


#endif // TEXTDATA_H
