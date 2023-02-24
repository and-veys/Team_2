#include "filefunction.h"
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include<QTextStream>
#include <QTextCodec>


FileFunction::FileFunction(QObject *parent) : QObject(parent)
{
    textFromFile = "";
}


/*
Открытие файла
Для передачи данных излучаем сигнал
*/
void FileFunction::slotOpenFile(){
    QString fileName = QFileDialog::getOpenFileName(nullptr,("Открыть файл"), "/", ("Тип файла (*.tm2)"));//Указали путь к файлу
    if(fileName.isEmpty())return;//Если имя файла не указано
    file.setFileName(fileName);
    if(file.open(QIODevice::ReadWrite)){



        QByteArray ba = file.readAll();//прочитали данные из файла
        textFromFile = QString::fromUtf8(ba);      //АМВ: борьба с кириллицей

        //qDebug() << ba;

        //qDebug() << textFromFile;


//        QTextCodec *codec = QTextCodec::codecForName("CP1251");
//        QString str = codec->toUnicode(file.readAll());
//fromLocal8bit()


        emit signalFileDataReady(&textFromFile);
    }
}


/*
Сохранить файл под текущим именем
*/
void FileFunction::slotSaveFile(QString *data){
    if(file.fileName()==":/help/Help.txt")return;//Если открыт файл спарвки, то выходим
    if(file.isOpen()){//Если файт был открыт, сохраняем его. Если файл был создан, то запускаем функцию сохранить как.
        if(file.openMode() == QIODevice::ReadOnly){//Если файл был открыт только для чтения, то выходим, иначе сохраняем его.
            qDebug()<<"ReadOnly";
            return;
        }
        else{
            //Переоткрываем файл с параметрами QIODevice::WriteOnly | QIODevice::Truncate чтобы перезаписать содержимое файла
            file.close();
            file.open(QIODevice::WriteOnly | QIODevice::Truncate);
            file.write(data->toUtf8().constData());
/*  АМВ: не работает с кирилицей
            qDebug()<<"Save";
            QTextStream writeStream(&file);
            writeStream << *data;
 */
         }
    }
    else{
        if(*data != "") slotSaveFileAs(data);//
   }
}

/*
Сохранить файл под новым именем
*/
void FileFunction::slotSaveFileAs(QString *data){
    QString fileName = QFileDialog::getSaveFileName(nullptr,("Сохраниеть как.."), "/", ("Тип файла (*.tm2)"));
    QFile file;
    file.setFileName(fileName);
    if(file.open(QIODevice::WriteOnly)){
        file.write(data->toUtf8().constData());
/*  АМВ: не работает с кирилицей
            QTextStream writeStream(&file);
            writeStream << *data;
 */

     }
}


/**/

/*
Закрытие файла
*/
void FileFunction::slotCloseFile(QString *data){
    slotSaveFile(data);
    file.close();
    emit signalFileCloseCompleet();

}


///*
// * Возможно ту функцию логично былобы реализовать в другом месте кода
// * руководствуясь тем, что при закрытии программы надо бы сохранить файл, пока оставлю сдесь как напоминашку
//Выход из программы
//*/
////void fileFunction::slotExitProgramm(QTextEdit* textEdit){
////    slotCloseFile(textEdit);
////    QApplication::quit();
////}


///*
//Создать новый документ
//файл при этом не создается
//Файл создастся при сохранении документа
//*/
//void FileFunction::slotCreateNewDoc(QTextEdit* textEdit){
//   textEdit->setEnabled(true);

//}




/**************************/
/*Как читать и писать текст с форматированием
 * пуская пока повисит...
void MainWindow::slotSaveFileAs(){
    QString fileName = QFileDialog::getSaveFileName(this,tr("Сохраниеть как.."), "/", tr("Тип файла (*.txt);;HTML files (*.html)"));
    QFile file;
    file.setFileName(fileName);
    if(file.open(QIODevice::WriteOnly)){
 //       QString text = ui->textEdit->toPlainText();
        QString text = ui->textEdit->toHtml();
        qDebug() << text;
        //QByteArray ba = text.toUtf8();
        //QByteArray ba = text;
        //file.write(ba,ba.size());
        QTextStream writeStream(&file);
        writeStream << text;
        file.close();
    }
}

void FileFunction::slotOpenFile(){
    QString fileName = QFileDialog::getOpenFileName(this,tr("Открыть файл"), "/", tr("Тип файла (*.txt);;HTML files (*.html)"));//Указали путь к файлу
    if(fileName.isEmpty())return;//Если
    file.setFileName(fileName);
    if(file.open(QIODevice::ReadWrite)){
        QByteArray text = file.readAll();//прочитали данные из файла
        ui->textEdit->setReadOnly(false);
//        ui->textEdit->setPlainText(text.data());//вывели в проле
qDebug() << text;
        ui->textEdit->append(text);
    }
}

*/
