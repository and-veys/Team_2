#include "textdata.h"
#include <QDebug>
TextData::TextData()
{
    //параметры спрятанного текста
    parametersHide = new ParameterHide("[...]", "<hide>", Qt::gray);

    //параметры важности текста в порядке значимости
    auto addImportance = [this](QString tag, QString nm, QColor col){
        QString key  = "<" + tag.toUpper() + ">";
        this->parametersImportance.insert(key, new ParameterImportance(nm, key, col));
    };
    addImportance("normal", "обычный текст",  Qt::black);
    addImportance("low_imp", "не важный текст",  Qt::green);
    addImportance("imp", "важный текст",  Qt::blue);
    addImportance("high_imp", "очень важный текст",  Qt::red);
}


TextData::~TextData()
{
    delete parametersHide;
    foreach(auto dt, parametersImportance)
        delete dt;
}
int ParameterImportance::count = 0;

ParameterImportance *TextData::getParameterImportance(const QString &key)
{
    return (parametersImportance.contains(key) ? parametersImportance.take(key) : parametersImportance.take("normal"));

}

QList<ParameterImportance *> TextData::getSortListImportance()      //сортировка важности по id
{
    QList<ParameterImportance *> list;
    foreach(auto dt, parametersImportance)
        list.push_back(dt);
    auto sort = [](ParameterImportance * a, ParameterImportance * b){return (a->id < b->id);};
    std::sort(list.begin(), list.end(), sort);
    return list;
}


ParametersTag::ParametersTag(QString tag, QColor col)
{
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
    id = count++;
}

ParameterImportance::~ParameterImportance()
{
    //qDebug() << nameImportance << id << " - " << count;
}


