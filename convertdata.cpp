/******************************************
В классе TextData надо строки
ParameterHide * parametersHide;
QMap<int, QString> hiddenString;
вынести в public или написать гетеры

************************************************/

#include "convertdata.h"
#include "textdata.h"
#include<QDebug>
#include<QTextCursor>
#include<QPlainTextEdit>


ConvertData::ConvertData(QObject *parent) : QObject(parent)
{

}
/*
 * конвертируем данные из текстового поля в строку для передачи в файл
 * на вход указатель на текстовое поле, указатель на класс TextData
*/
void ConvertData::converterData(QPlainTextEdit *edtWin, TextData *textData, QString *result){
    qDebug() << "DEBUG";
    QTextCursor cursor;                             //Создаем экземпляр курсора
    QString strTag ;                                //текущий тэг текста
    QString strTagPref = "";                        //предыдущий тэг текста
    ParameterHide *parametr;                        //Указатель на параметр для текущего символа
    QString dataString = edtWin->toPlainText();     //Текст из окна
    QString resultString = "";                      //Строка для записи текста с тегами
    QString strHidenText = "";                      //Строка для скрытого текста
    int sizeHidenPicto = 5-1;                       //Размер пиктограммы для скрытого текста(5 длина минус текущая позиция)
    int currentPosition;                            //Текущее положение курсора в строке
    int end;                                        //положение конца строки

    cursor = edtWin->textCursor();//говорим, что этот экземпляр отосится к нашему текстовому окну
    cursor.setPosition(1);//Курсор на начало строки
//    currentPosition  = cursor.selectionStart();
    currentPosition = cursor.position();
    end = 10;//cursor.selectionEnd();
    /********Хочу получить тэг скрытого текста**********/
    qDebug() << QString(tr("currentPosition %1, End %2").arg(currentPosition).arg(cursor.selectionEnd()));
  //  cursor.setPosition(1);

    while(currentPosition <= end) {
        parametr = textData->getParameterHide();//Параметр текущего символа
        strTag =   parametr->getTag(cursor.charFormat());//Получаем тэг текущего символа
             qDebug()<<"Tag " << strTag << "currentPosition " << currentPosition<< " "<< dataString[currentPosition];

        if(strTagPref.compare(strTag) == 0){//если текущий тег равен предыдущему
            resultString.append(dataString[currentPosition]);
            cursor.movePosition(QTextCursor::NextCharacter);
        }
        else{
            strTagPref.clear();
            strTagPref.append(strTag);
            resultString.append(strTag);
            if(strTag.compare("<hide>") == 0){//Если нашли скрытый текст

                strHidenText.append(strHidenText.append(textData->hiddenString.value(textData->parametersHide->getHideKey(cursor))));
                qDebug() << "Скрытый текст " << strHidenText;
                resultString.append(strHidenText);
                strHidenText.clear();
                currentPosition += sizeHidenPicto;//переместили счетчик на размер пиктограммы
                //Вот так по-индусски перешагнем пиктограмму скрытого текста...
                for(int i = 0; i <= sizeHidenPicto; i++)
                    cursor.movePosition(QTextCursor::NextCharacter);
            }
            else
            {
                resultString.append(dataString[currentPosition]);
                cursor.movePosition(QTextCursor::NextCharacter);
            }
        }
        //        qDebug() << strTag << QString::number(cursor.position()) << dataString[start];
        //qDebug() << "current position "<< start << " " << end << "strTagPref" << strTagPref << "strTag" << strTag;
        currentPosition++;
    }
    qDebug() << "Result " << resultString;
    result->append(resultString);
}

/*
 * функция загрузки из файла в текстовое поле
 * на вход указатель на строку, указатель на текстовое поле, указатель на класс TextData
 *
 * вставляем текст
 * План такой,
 * Ищем начало тега '<'
 * Пишем в gettingTag пока не найдем конец тэга '>'. Пишем включая скобки
 * Определяем что за тег, и устанавливаем параметры курсора
 * Теперь пока не кончится строка или не найдем начало нового тега выводим текст на форму

*/
void ConvertData::loadData(QString *gettingString, QPlainTextEdit *edtWin, TextData *textData){
    QTextCursor cursor;                             //Создаем экземпляр курсора
//    QString *gettingString = &resultString;//Указатель на полученную строку
    QString gettingTag = "";//полученный тэг
//    int stringSize = resultString.size() - 1;//длина прочитанной строки (-1 это ткинули терминаор)
    int stringSize = gettingString->size() - 1;//длина прочитанной строки (-1 это ткинули терминаор)
    qDebug() << "stringSize" << stringSize;
    edtWin->clear();
    int position = 0;
    cursor = edtWin->textCursor();//говорим, что этот экземпляр отосится к нашему текстовому окну
    cursor.setPosition(0);//Курсор на начало

    while(position < (stringSize)){
        //Ищем начало тега
        if(gettingString->at(position) == '<'){//если нашли начало тега, то записываем его
            //        qDebug() << gettingString->at(position);
            gettingTag.append(gettingString->at(position));
            position++;
            while(gettingString->at(position) != '>'){
                //            qDebug() << gettingString->at(position);
                gettingTag.append(gettingString->at(position));
                position++;
            }

            gettingTag.append(gettingString->at(position));
            position++;
        }
        //    qDebug() << "detect Tag "<<gettingTag;


        if(!gettingTag.isEmpty()){//если нашли тег, то пишем его
            QTextCharFormat ch = cursor.charFormat();
            textData->getParameterImportance(gettingTag)->setParameters(ch);
            cursor.setCharFormat(ch);
            //        qDebug() << "setTag "<<gettingTag;
            gettingTag.clear();
            //        qDebug() << "gettingTag is clear "<<gettingTag;
        }
        else{//Если тега нет то устанавливаем тег "<NORMAL>"
            QTextCharFormat ch = cursor.charFormat();
            textData->getParameterImportance("<NORMAL>")->setParameters(ch);
            cursor.setCharFormat(ch);
            gettingTag.clear();
        }
        //    qDebug() << "Start write string ";
        //Пока не найдем начало нового тега пишем текст в поле
        while(gettingString->at(position) != '<' && position < (stringSize)  ){
            if(gettingString->at(position) != '<' ){
                cursor.insertText(gettingString->at(position));
                //            qDebug() << "stringSize " << stringSize << "position " << position << "data "<< gettingString->at(position);
                position++;
            }

        }
        //    qDebug() << "i am here ";
        //position++;
    }
}
