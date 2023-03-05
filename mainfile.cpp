#include "mainfile.h"

#include <QFileDialog>
#include <QDebug>

MainFile::MainFile():QFile()
{

}

MainFile::~MainFile()
{
    close();
}

bool MainFile::create(QWidget *wnd, bool save)
{
    QString fileName = (save ?
            QFileDialog::getSaveFileName(wnd, "Сохранить как...", QDir::currentPath(), "Тип файла (*.tm2)") :
            QFileDialog::getOpenFileName(wnd, "Открыть файл...", QDir::currentPath(), "Типы файла (*.tm2; *.txt)"));
    if(fileName == "") {
        emit sendMessage("Операция отменена");
        return false;
    }
    setFileName(fileName);
    return true;
}
bool MainFile::save(const QString &data)
{
    close();
    if(!open(QIODevice::WriteOnly)) {
        emit sendMessage("Ошибка открытия файла");
        return false;
    }
    QFileInfo fn(fileName());
    QDir::setCurrent(fn.absoluteDir().path());
    emit sendWindowCaption(fn.fileName());
    write(data.toUtf8().constData());    
    emit sendMessage("Файл сохранен успешно");
    return true;
}

bool MainFile::load(QString &data)
{
    if(!open(QIODevice::ReadOnly)) {
        emit sendMessage("Ошибка открытия файла");
        return false;
    }
    QByteArray ba = readAll();
    data = QString::fromUtf8(ba);
    QFileInfo fn(fileName());
    emit sendWindowCaption(fn.fileName());
    emit sendMessage("Файл загружен успешно");
    return true;
}

bool MainFile::isTM2File()
{
    QStringList lst = fileName().split(".");
    return (lst[lst.length() - 1].toLower() == "tm2");
}

