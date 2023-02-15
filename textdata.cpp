#include "textdata.h"
#include <QDebug>
#include <QPlainTextEdit>
TextData::TextData() : QObject()
{
    //параметры спрятанного текста
    parametersHide = new ParameterHide("[...]", "<hide>", Qt::gray);

    //параметры важности текста в порядке значимости
    auto addImportance = [this](QString tag, QString nm, QColor col){
        QString key  = "<" + tag.toUpper() + ">";
        this->parametersImportance.insert(key, new ParameterImportance(nm, key, col));
    };
    addImportance("normal", "Обычный текст",  Qt::white);
    addImportance("low_imp", "Не важный текст",  Qt::green);
    addImportance("imp", "Важный текст",  Qt::blue);
    addImportance("high_imp", "Очень важный текст",  Qt::red);
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
    return (parametersImportance.contains(key) ? parametersImportance.value(key) : parametersImportance.value("normal"));

}

QList<ParameterImportance *> TextData::getSortListImportance()      //сортировка важности по id
{
    QList<ParameterImportance *> list;
    foreach(auto dt, parametersImportance)
        list.push_back(dt);
    auto sort = [](ParameterImportance * a, ParameterImportance * b){return (a->getId() < b->getId());};
    std::sort(list.begin(), list.end(), sort);
    return list;
}

void TextData::setImportance(QPlainTextEdit * wnd, QString & tag)       //TODO обработать сигнал ошибки вставки формата
{
    QTextCursor cursor = wnd->textCursor();
    if(!cursor.hasSelection()) return;
    if(parametersHide->hasHideText(cursor)) {                           //TODO протестировать
        emit errorSetFormat("There is a hidden part in the selection text");
        return;
    }
    QTextCharFormat ch = cursor.charFormat();
    getParameterImportance(tag)->setParameters(ch);



    cursor.setCharFormat(ch);
    cursor.setPosition(cursor.position());
    wnd->setTextCursor(cursor);
}

//class ParametersTag -------------------------------------------------------------------------

ParametersTag::ParametersTag(QString tag, QColor col): color(col), tag(tag)
{

}

ParametersTag::~ParametersTag()
{

}

void ParametersTag::setParameters(QTextCharFormat &ch)
{
    int r, g, b;
    color.getRgb(&r, &g, &b);
    QColor col = ((r*0.299 + g*0.587 + b*0.114) > 140 ? Qt::black : Qt::white);
    ch.setForeground(col);
    ch.setBackground(color);
    ch.setProperty(1, tag);
}
//class ParameterHide --------------------------------------------------------------------------
ParameterHide::ParameterHide(QString txt, QString tag, QColor col) : ParametersTag(tag, col)
{
    replacingText = txt;
}

ParameterHide::~ParameterHide()
{
    //qDebug() << replacingText;

}

bool ParameterHide::hasHideText(QTextCursor cursor)
{
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    while(start <= end) {
        if(getTag(cursor.charFormat()) == getTag())
            return true;
        cursor.movePosition(QTextCursor::NextCharacter);
        ++start;
    }
    return false;
}
//class ParameterImportance --------------------------------------------------------------------------
ParameterImportance::ParameterImportance(QString name, QString tag, QColor col) : ParametersTag(tag, col)
{
    nameImportance = name;
    id = count++;
}

ParameterImportance::~ParameterImportance()
{
    //qDebug() << nameImportance << id << " - " << count;
}
//---------------------------------------------------------------------------------------------------

