#include "textdata.h"
#include <QDebug>
TextData::TextData()
{
    data = "";
    parametersHide = new ParameterHide("[...]");
    parametersImportance.insert("low_imp", new ParameterImportance("не важно", "low_imp", QColor(0,255,0)));
    parametersImportance.insert("imp", new ParameterImportance("важно", "imp", QColor(0,0,255)));
    parametersImportance.insert("high_imp", new ParameterImportance("очень важно", "high_imp", QColor(255,0,0)));
}


TextData::~TextData()
{
    //qDebug() << getParameterImportance("imp")->nameImportance;
    //qDebug() << getParameterHide()->replacingText;

    delete parametersHide;
    foreach(auto dt, parametersImportance)
        delete dt;
}

ParameterHide::ParameterHide(QString txt, QString tag)
{
    replacingText = txt;
    startTag = "<" + tag.toUpper() + ">";
    endTag = "</" + tag.toUpper() + ">";
}

ParameterImportance::ParameterImportance(QString name, QString tag, QColor col)
{
    nameImportance = name;
    color = col;
    startTag = "<" + tag.toUpper() + ">";
    endTag = "</" + tag.toUpper() + ">";
}
