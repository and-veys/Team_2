#include "aboutus.h"

aboutus::aboutus(QWidget *parent) : QMainWindow(parent)
{
}

void aboutus::slotAboutUs()
{
    aboutus::file.setFileName("e:\\GB\\Projects\\Team_2\\aboutUs.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        QMessageBox::about(this, "О нас", stream.readAll());
    }

}
