#ifndef CONVERTDATA_H
#define CONVERTDATA_H

#include <QObject>
#include<QPlainTextEdit>


class ParameterHide;
class EditWindow;
class TextData;

class ConvertData : public QObject
{
    Q_OBJECT
public:
    explicit ConvertData(QObject *parent = nullptr);


    void loadData(QString *gettingString, QPlainTextEdit *edtWin, TextData *textData);
    void converterData(QPlainTextEdit *edtWin, TextData *textData, QString *result);
signals:

public slots:
};

#endif // CONVERTDATA_H
