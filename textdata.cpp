#include "textdata.h"
#include <QDebug>
#include <QPlainTextEdit>
#include <QThread>
TextData::TextData() : QObject()
{
    //параметры спрятанного текста
    parametersHide = new ParameterHide("[...]", ParametersTag::createTag("hide"), Qt::black);

    //параметры важности текста в порядке значимости
    auto addImportance = [this](QString tag, QString nm, QColor col){
        QString key  = ParametersTag::createTag(tag);
        parametersImportance.insert(key, new ParameterImportance(nm, key, col));
    };
    addImportance("normal", "Обычный текст",  Qt::white);
    addImportance("low_imp", "Не важный текст",  Qt::yellow);
    addImportance("imp", "Важный текст",  Qt::green);
    addImportance("high_imp", "Очень важный текст",  Qt::red);

    errorTexts.insert(NOT_SELECT, "Ничего не выбрано");
    errorTexts.insert(HIDE_SELECT, "Имеется спрятанная часть в выделенном тексте");
    errorTexts.insert(SOME_SELECT, "Что-то выделено");
    errorTexts.insert(NOT_HIDE, "Это не спратанный текст");
    errorTexts.insert(NOT_NORMAL, "Это не обычный текст");

    searchTexts.insert(FIND_STR, "Строка найдена");
    searchTexts.insert(STOP_UP, "Поиск достиг верхнего предела");
    searchTexts.insert(STOP_DOWN, "Поиск достиг нижнего предела");
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

void TextData::sendSearchSignal(TextData::searchEnum key)
{
    QString mess = (searchTexts.contains(key) ? searchTexts.value(key) : "Undefined result");
    emit errorSetFormat(mess);
}

ParametersTag *TextData::getParameterFormat(const QTextCursor & cursor)
{
    QString tag = ParametersTag::getTag(cursor.charFormat());
    if(tag == getParameterHide()->getTag()) return getParameterHide();
    return getParameterImportance(tag);
}


ParameterImportance *TextData::getParameterImportance(const QString &key)
{
    return (parametersImportance.contains(key) ?
                parametersImportance.value(key) :
                parametersImportance.value(ParametersTag::createTag("normal")));

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
    QString ins = hiddenString.take(parametersHide->getHideKey(cursor));
    int start = cursor.selectionStart();
    if(cursor.selectedText() != parametersHide->getReplacingText()) {
        ins = cursor.selectedText()[0] + ins;
        ++start;
    }
    QTextCharFormat ch = cursor.charFormat();
    getNormalText()->setParameters(ch);

    cursor.insertText(hiddenString.take(parametersHide->getHideKey(cursor)), ch);

    cursor.insertText(ins, ch);
    cursor.setPosition(start, QTextCursor::KeepAnchor);
    wnd->setTextCursor(cursor);
}

void TextData::searchFormatString(QPlainTextEdit *wnd, const QString &tag, bool next)
{
    QTextCursor cursor = wnd->textCursor();
    cursor.clearSelection();
    ParametersTag * current;
    bool first = true;
    while(true) {
        current = getParameterFormat(cursor);
        current->findCharsFormat(cursor);
        if(current->getTag() == tag && !first) {
            sendSearchSignal(FIND_STR);
            break;
        }
        first = false;
        if(next) {
            cursor.clearSelection();
            if(!cursor.movePosition(QTextCursor::NextCharacter)) {
                 sendSearchSignal(STOP_DOWN);
                break;
            }
        }
        else {
            cursor.setPosition(cursor.selectionStart());
            if(cursor.position() == 0) {
                sendSearchSignal(STOP_UP);
                break;
            }
        }
    }
    wnd->setTextCursor(cursor);
}

void TextData::searchTextString(QPlainTextEdit *wnd, const QString &str, bool next)
{
    QTextCursor cursor = wnd->textCursor();
    if(!next && cursor.selectedText() == str)
        cursor.setPosition(cursor.selectionStart()-1);
    if(!next && cursor.position() == 0)
        sendSearchSignal(STOP_UP);
    else
    {
        cursor.clearSelection();
        QString txt = wnd->toPlainText();
        int start = (next ? txt.indexOf(str, cursor.position()) : txt.lastIndexOf(str, cursor.position()));
        if(start == -1) {
            if(next) {
                cursor.setPosition(txt.length());
                sendSearchSignal(STOP_DOWN);
            }
            else {
                cursor.setPosition(0);
                sendSearchSignal(STOP_UP);
            }
        }
        else {
            cursor.setPosition(start);
            cursor.setPosition(start + str.length(), QTextCursor::KeepAnchor);
            sendSearchSignal(FIND_STR);
        }
    }
    wnd->setTextCursor(cursor);
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
    if((mod == Qt::ControlModifier && key == 90)) return true; //запрет ctrl+Z
    if(event->text().length() == 0) return false;                       //всякая навигация

    if(cursor.hasSelection()) {
        if(parametersHide->hasCharsFormat(cursor)) {
            sendErrorSignal(HIDE_SELECT);
            return true;
        }
        return false;
    }
    QTextCharFormat ch;
    int flag = parametersHide->getPlaceCursor(cursor, ch);    
    if(flag == NOT) return false;
    if(flag == CENTER) return true;
    if(key == Qt::Key_Delete) return (flag & START);
    if(key == Qt::Key_Backspace) return (flag & END);
    if(flag == START) return false;

    if(flag & NORMAL || getNormalText()->getTag(ch) == "")
        getNormalText()->setParameters(ch);

    cursor.insertText(event->text(), ch);
    return true;
}

QString TextData::convertToString(QPlainTextEdit *wnd)
{
    QTextCursor cursor = QTextCursor(wnd->textCursor());
    QString tag_hide = parametersHide->getTag();
    QString tag_cur;
    QString result = "";
    int hideKey;
    bool br;
    ParametersTag * current;
    cursor.setPosition(0);
    while(true) {
        tag_cur = parametersHide->getTag(cursor.charFormat());
        if(tag_hide == tag_cur) {
            current = parametersHide;
            hideKey = parametersHide->getHideKey(cursor);
        }
        else
            current = getParameterImportance(tag_cur);
        br = (cursor.positionInBlock() == 0 && cursor.position() != 0);
        if(current->findCharsFormat(cursor)) {
            if(tag_hide == tag_cur) {
                if(br) result += QString(0x2029);
                result += (tag_cur + getHiddenString(hideKey));
            }
            else
                result += (tag_cur + cursor.selectedText());
            cursor.clearSelection();
        }
        if(!cursor.movePosition(QTextCursor::NextCharacter)) break;
    }
    return result;
}

void TextData::convertFromString(const QString &str, QPlainTextEdit *wnd)
{
    QTextCursor cursor = QTextCursor(wnd->textCursor());
    QString tag_hide = parametersHide->getTag();
    QString tag_cur;
    QStringList lst = str.split(ParametersTag::startTag());
    QStringList current;
    QTextCharFormat ch;
    foreach(auto el, lst) {
       current = el.split(ParametersTag::endTag());
       if(current.length() == 1)  {
           tag_cur = getNormalText()->getTag();
           current.push_front(tag_cur);
       }
       else
           tag_cur = ParametersTag::createTag(current.at(0));
       if(current.at(1).length() == 0) continue;
       ch = cursor.charFormat();
       if(tag_cur == tag_hide) {
           hiddenString.insert(parametersHide->setParameters(ch), current.at(1));
           cursor.insertText(parametersHide->getReplacingText(), ch);
       }
       else {
           getParameterImportance(tag_cur)->setParameters(ch);
           cursor.insertText(current.at(1), ch);
       }
    }
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
    int hideKey = getHideKey(cursor);
    while(tag == cur_tag && hideKey == getHideKey(cursor)) {
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
int ParameterHide::id = 1;

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

