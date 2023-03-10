#ifndef TEXTDATA_H
#define TEXTDATA_H

#include <QColor>
#include <QMap>
#include <QObject>
#include <QTextCharFormat>
#include <QTextCursor>

class QPlainTextEdit;
class QKeyEvent;
class ParametersTag;
class ParameterHide;
class ParameterImportance;

class TextData : public QObject
{
    Q_OBJECT
public:
    TextData();
    ~TextData();

    enum errorEnum {
        NOT_SELECT = 0,
        HIDE_SELECT,
        SOME_SELECT,
        NOT_HIDE,
        NOT_NORMAL
    };
    enum searchEnum {
        FIND_STR = 0,
        STOP_UP,
        STOP_DOWN
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
    QMap<searchEnum, QString> searchTexts;
    ParameterHide * parametersHide;
    void sendErrorSignal(errorEnum key);
    void sendSearchSignal(searchEnum key);
    ParameterImportance * getNormalText(){return getParameterImportance("!");};
    ParametersTag * getParameterFormat(const QTextCursor & cursor);
public:     
    ParameterImportance * getParameterImportance(const QString &key);    
    ParameterHide * getParameterHide(){return parametersHide;};
    QString getHiddenString(int key) {return (hiddenString.contains(key) ? hiddenString.value(key): "");};
    QList<ParameterImportance *> getSortListImportance();
    void setImportance(QPlainTextEdit * wnd, QString & tag);
    void hideText(QPlainTextEdit * wnd);
    void showText(QPlainTextEdit * wnd);
    void clear(){hiddenString.clear();};
    void searchFormatString(QPlainTextEdit * wnd, const QString & tag, bool next);
    void searchTextString(QPlainTextEdit * wnd, const QString & str, bool next);
signals:
    void errorSetFormat(QString);
public slots:
    bool isForbiddenKey(QKeyEvent * event);
    QString convertToString(QPlainTextEdit * wnd);
    void convertFromString(const QString & str, QPlainTextEdit * wnd);
};


class ParametersTag {
public:
    ParametersTag(QString tag, QColor col);
    ~ParametersTag();
    QString getTag(){return tag;};
    static QString getTag(QTextCharFormat ch){return ch.property(1).toString();};
    QColor getColor(){return color;};
    void setParameters(QTextCharFormat & ch);
    bool hasCharsFormat(QTextCursor cursor){return isCharFormat(cursor, false);};       //хоть один символ формата объекта
    bool allCharsFormat(QTextCursor cursor){return isCharFormat(cursor, true);};        //все символы формата объекта
    bool findCharsFormat(QTextCursor& cursor);                  //выделение строки текущего формат
    int findLimitFormat(QTextCursor cursor, bool next);
    static QString createTag(const QString & tag){return startTag() + tag.toUpper() + endTag();};
    static QString startTag(){return "<" + QString(1);};
    static QString endTag(){return QString(1) + ">";};
    int getHideKey(QTextCursor & cursor){return cursor.charFormat().property(2).toInt();};
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
    QString getNameHide(){return "Скрытый текст";};
private:
    QString replacingText;
    static int id;        //для уникального идентификатора спрятанного текста

};


#endif // TEXTDATA_H
