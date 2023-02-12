#include "textdata.h"
#include <QDebug>
TextData::TextData()
{
    //параметры спрятанного текста
    parametersHide = new ParameterHide("[...]", "hide", Qt::gray);

    //параметры важности текста
    parametersImportance.insert("normal", new ParameterImportance("обычный текст", "normal", Qt::black));
    parametersImportance.insert("low_imp", new ParameterImportance("не важный текст", "low_imp", Qt::red));
    parametersImportance.insert("imp", new ParameterImportance("важный текст", "imp", Qt::blue));
    parametersImportance.insert("high_imp", new ParameterImportance("очень важный текст", "high_imp", Qt::green));
}


TextData::~TextData()
{
    delete parametersHide;
    foreach(auto dt, parametersImportance)
        delete dt;
}
ParameterImportance *TextData::getParameterImportance(const QString &key)
{
    return (parametersImportance.contains(key) ? parametersImportance.take(key) : parametersImportance.take("normal"));

}
ParametersTag::ParametersTag(QString tag, QColor col)
{
    startTag = "<" + tag.toUpper() + ">";
    lineFormat.setBackground(col);
    lineFormat.setProperty(1, tag);
}

ParametersTag::~ParametersTag()
{
   //qDebug() << startTag;

}

ParameterHide::ParameterHide(QString txt, QString tag, QColor col) : ParametersTag(tag, col)
{
    replacingText = txt;
}

ParameterHide::~ParameterHide()
{
    //qDebug() << replacingText;

}

ParameterImportance::ParameterImportance(QString name, QString tag, QColor col) : ParametersTag(tag, col)
{
    nameImportance = name;
}

ParameterImportance::~ParameterImportance()
{
     //qDebug() << nameImportance;
}


