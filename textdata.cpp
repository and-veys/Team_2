#include "textdata.h"
#include <QDebug>
#include <QPlainTextEdit>
#include <QThread>
TextData::TextData() : QObject()
{
    //параметры спрятанного текста
    parametersHide = new ParameterHide("[...]", "<hide>", Qt::black);

    //параметры важности текста в порядке значимости
    auto addImportance = [this](QString tag, QString nm, QColor col){
        QString key  = "<" + tag.toUpper() + ">";
        parametersImportance.insert(key, new ParameterImportance(nm, key, col));
    };
    addImportance("normal", "Обычный текст",  Qt::white);
    addImportance("low_imp", "Не важный текст",  Qt::yellow);
    addImportance("imp", "Важный текст",  Qt::green);
    addImportance("high_imp", "Очень важный текст",  Qt::red);

    errorTexts.insert(NOT_SELECT, "Nothing is selected");
    errorTexts.insert(HIDE_SELECT, "There is a hidden part in the selection text");
    errorTexts.insert(SOME_SELECT, "Something is selected");
    errorTexts.insert(NOT_HIDE, "This isn`t a hidden part");
    errorTexts.insert(NOT_NORMAL, "There isn't a simple part");
}


TextData::~TextData()
{
    delete parametersHide;
    foreach(auto dt, parametersImportance)
        delete dt;
}

void TextData::sendErrorSignal(errorEnum key)
{
    QString mess = (errorTexts.contains(key) ? errorTexts.value(key) : "Undefined error");
    emit errorSetFormat(mess);
}


ParameterImportance *TextData::getParameterImportance(const QString &key)
{
    return (parametersImportance.contains(key) ? parametersImportance.value(key) : parametersImportance.value("<NORMAL>"));

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

void TextData::setImportance(QPlainTextEdit * wnd, QString & tag)
{
    QTextCursor cursor = wnd->textCursor();
    if(!cursor.hasSelection()) {
        sendErrorSignal(NOT_SELECT);
        return;
    }
    if(parametersHide->hasCharsFormat(cursor)) {
        sendErrorSignal(HIDE_SELECT);
        return;
    }
    QTextCharFormat ch = cursor.charFormat();
    getParameterImportance(tag)->setParameters(ch);
    cursor.setCharFormat(ch);
    cursor.setPosition(cursor.position());
    wnd->setTextCursor(cursor);
}

void TextData::showText(QPlainTextEdit *wnd)
{
    QTextCursor cursor = wnd->textCursor();
    if(cursor.hasSelection()) {
        sendErrorSignal(SOME_SELECT);
        return;
    }
    if(!parametersHide->findCharsFormat(cursor)) {
        sendErrorSignal(NOT_HIDE);
        return;
    }
    wnd->setTextCursor(cursor);
    QTextCharFormat ch = cursor.charFormat();
    getNormalText()->setParameters(ch);
    cursor.insertText(hiddenString.take(parametersHide->getHideKey(cursor)), ch);
}

void TextData::hideText(QPlainTextEdit *wnd)
{
    QTextCursor cursor = wnd->textCursor();
    if(!cursor.hasSelection()) {
        sendErrorSignal(NOT_SELECT);
        return;
    }
    if(parametersHide->hasCharsFormat(cursor)) {
        sendErrorSignal(HIDE_SELECT);
        return;
    }
    if(!getNormalText()->allCharsFormat(cursor)) {
        sendErrorSignal(NOT_NORMAL);
        return;
    }
    QTextCharFormat ch = cursor.charFormat();
    int pos = (cursor.selectionStart() == cursor.position() ? cursor.position() : -1);
    hiddenString.insert(parametersHide->setParameters(ch), cursor.selectedText());
    cursor.insertText(parametersHide->getReplacingText(), ch);
    if(pos >= 0) cursor.setPosition(pos);
    wnd->setTextCursor(cursor);
}

bool TextData::isForbiddenKey(QKeyEvent * event)
{
    QPlainTextEdit * wnd = qobject_cast<QPlainTextEdit *>(sender());
    QTextCursor cursor = wnd->textCursor();
    Qt::KeyboardModifiers mod = event->modifiers();
    int key = event->key();

    if((mod == Qt::ControlModifier && key == 86) || (mod == Qt::ShiftModifier && key == Qt::Key_Insert)) return true; //запрет paste
    if(key == 60 || key == 62) return true;                             //запрет на '<' и '>'
    if(event->text().length() == 0) return false;                       //всякая навигация

    if(cursor.hasSelection()) {
        sendErrorSignal(HIDE_SELECT);
        return parametersHide->hasCharsFormat(cursor);
    }
    QTextCharFormat ch;
    int flag = parametersHide->getPlaceCursor(cursor, ch);
    if(flag == NOT || flag == START) return false;
    if(flag == CENTER) return true;
    if(key == Qt::Key_Delete) return (flag & START);
    if(key == Qt::Key_Backspace) return (flag & END);

    if(flag & NORMAL || getNormalText()->getTag(ch) == "")
        getNormalText()->setParameters(ch);

    cursor.insertText(event->text(), ch);
    return true;
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

bool ParametersTag::findCharsFormat(QTextCursor &cursor)
{
    QString tag = getTag();
    QString cur_tag = tag;

    int start = findLimitFormat(cursor, false);
    int end = findLimitFormat(cursor, true);
    if(start == end) return false;
    cursor.setPosition(start);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    return true;
}

int ParametersTag::findLimitFormat(QTextCursor cursor, bool next)
{
    QTextCursor::MoveOperation move = (next ? QTextCursor::NextCharacter : QTextCursor::PreviousCharacter);
    QString tag = getTag();
    QString cur_tag = tag;
    if(getTag(cursor.charFormat()) != tag) return cursor.position();

    while(tag == cur_tag) {
        if(!cursor.movePosition(move)) return cursor.position();
        cur_tag = getTag(cursor.charFormat());
    }
    return cursor.position() - (next ? 1 : 0);
}

bool ParametersTag::isCharFormat(QTextCursor & cursor, bool all) {

    int start = cursor.selectionStart()+1;
    int end = cursor.selectionEnd();
    cursor.setPosition(start);
    while(start <= end) {
        if((getTag(cursor.charFormat()) == getTag()) != all)
            return !all;
        cursor.movePosition(QTextCursor::NextCharacter);
        ++start;
    }
    return all;
}




//class ParameterHide --------------------------------------------------------------------------
int ParameterHide::id = 0;

ParameterHide::ParameterHide(QString txt, QString tag, QColor col) : ParametersTag(tag, col)
{
    replacingText = txt;
}

ParameterHide::~ParameterHide()
{

}

int ParameterHide::setParameters(QTextCharFormat &ch)
{
    ParametersTag::setParameters(ch);
    ch.setProperty(2, id);
    return id++;
}

int ParameterHide::getPlaceCursor(QTextCursor cursor, QTextCharFormat &ch)
{
    ch = cursor.charFormat();
    QString tag = getTag();
    QString cur_tag = getTag(ch);

    if(cursor.positionInBlock() == 0)
        return (tag == cur_tag ? TextData::START | TextData::NORMAL : TextData::NORMAL);
    if(!cursor.movePosition(QTextCursor::NextCharacter))
        return (cur_tag == tag ? TextData::END | TextData::NORMAL : TextData::NOT);

    ch = cursor.charFormat();
    QString new_tag = getTag(ch);
    if(cur_tag != tag && new_tag == tag) return TextData::START;
    if(cur_tag == tag && new_tag == tag) return TextData::CENTER;
    if(cur_tag == tag && new_tag != tag) return TextData::END;
    return TextData::NOT;
}

//class ParameterImportance --------------------------------------------------------------------------

int ParameterImportance::count = 0;

ParameterImportance::ParameterImportance(QString name, QString tag, QColor col) : ParametersTag(tag, col)
{
    nameImportance = name;
    id = count++;
}

ParameterImportance::~ParameterImportance()
{

}
//---------------------------------------------------------------------------------------------------

