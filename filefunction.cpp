#include "filefunction.h"
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QFile>

fileFunction::fileFunction()
{

}

/**/
void fileFunction::slotSaveFile(QString* textEdit){
    if(file.fileName()==":/help/Help.txt")return;//Если открыт файл спарвки, то выходим
    if(file.isOpen()){//Если файт был открыт, сохраняем его. Если файл был создан, то запускаем функцию сохранить как.
        if(file.openMode() == QIODevice::ReadOnly){//Если файл был открыт только для чтения, то выходим, иначе сохраняем его.
            qDebug()<<"ReadOnly";
            return;
        }
        else{
            qDebug()<<"Save";
            QByteArray ba = textEdit->toUtf8();
            file.write(ba,ba.size());
       }
    }
    else{
        if(textEdit->size() > 0) slotSaveFileAs(textEdit);//
   }
}

/**/
void fileFunction::slotSaveFileAs(QString* textEdit){
    QString fileName = QFileDialog::getSaveFileName(nullptr,("Сохраниеть как.."), "/", ("Тип файла (*.txt)"));
    QFile file;
    file.setFileName(fileName);
    if(file.open(QIODevice::WriteOnly)){
        QByteArray ba = textEdit->toUtf8();
        file.write(ba,ba.size());
    }
}


/*
Открытие файла

!!!!Аргументом метода наверное нужен указатель на текстовой поле, куда выводить полученный из файла текст

*/
void fileFunction::slotOpenFile(QTextEdit* textEdit){
    QString fileName = QFileDialog::getOpenFileName(nullptr,("Открыть файл"), "/", ("Тип файла (*.txt)"));//Указали путь к файлу
    if(fileName.isEmpty())return;//Если имя файла не указано
    file.setFileName(fileName);
    if(file.open(QIODevice::ReadWrite)){
        QByteArray text = file.readAll();//прочитали данные из файла
        textEdit->setReadOnly(false);
        textEdit->setPlainText(text.data());//вывели в проле
    }
}

/*
Закрытие файла

*/
void fileFunction::slotCloseFile(QTextEdit* textEdit ){
    QString str = textEdit->toPlainText();
    slotSaveFile(&str);
    file.close();
    textEdit->clear();
    textEdit->setEnabled(false);

}

/*
 * Возможно ту функцию логично былобы реализовать в другом месте кода
 * руководствуясь тем, что при закрытии программы надо бы сохранить файл, пока оставлю сдесь как напоминашку
Выход из программы
*/
//void fileFunction::slotExitProgramm(QTextEdit* textEdit){
//    slotCloseFile(textEdit);
//    QApplication::quit();
//}


/*
Создать новый документ
файл при этом не создается
Файл создастся при сохранении документа
*/
void fileFunction::slotCreateNewDoc(QTextEdit* textEdit){
   textEdit->setEnabled(true);

}



/*
Открыть только для чтения
*/
void fileFunction::slotOpenForReadOnly_clicked(QTextEdit* textEdit)
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,("Открыть файл"), "/", ("Тип файла (*.txt)"));//Указали путь к файлу
    if(fileName.isEmpty())return;//Если
    file.setFileName(fileName);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray text = file.readAll();//прочитали данные из файла
        textEdit->setPlainText(text.data());//вывели в проле
        textEdit->setReadOnly(true);
    }
}

