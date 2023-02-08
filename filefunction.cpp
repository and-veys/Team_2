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
void fileFunction::slotSaveFile(QTextEdit* textEdit){
    if(file.fileName()==":/help/Help.txt")return;//Если открыт файл спарвки, то выходим
    if(file.isOpen()){//Если файт был открыт, сохраняем его. Если файл был создан, то запускаем функцию сохранить как.
        if(file.openMode() == QIODevice::ReadOnly){//Если файл был открыт только для чтения, то выходим, иначе сохраняем его.
            qDebug()<<"ReadOnly";
            return;
        }
        else{
            qDebug()<<"Save";
            QString text = textEdit->toPlainText();
            QByteArray ba = text.toUtf8();
            file.write(ba,ba.size());
       }
    }
    else{
        if(textEdit->toPlainText() != "") slotSaveFileAs(textEdit);//
   }
}

/**/
void fileFunction::slotSaveFileAs(QTextEdit* textEdit){
    QString fileName = QFileDialog::getSaveFileName(nullptr,("Сохраниеть как.."), "/", ("Тип файла (*.txt)"));
    QFile file;
    file.setFileName(fileName);
    if(file.open(QIODevice::WriteOnly)){
        QString text = textEdit->toPlainText();
        QByteArray ba = text.toUtf8();
        file.write(ba,ba.size());
    }
}


/*
Открытие файла
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
    slotSaveFile(textEdit);
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

