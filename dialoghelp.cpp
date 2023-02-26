#include "dialoghelp.h"

#include <QCommonStyle>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QPushButton>



DialogHelp::DialogHelp(QString & type, QWidget * parent) : QDialog(parent)
{
    resize(500,300);
    setWindowTitle("Окно справки");
    QGridLayout * layout = new QGridLayout(this);
    QPlainTextEdit * wnd = new QPlainTextEdit(this);
    QPushButton * but = new QPushButton(this);
    QCommonStyle style;
    but->setIcon(style.standardIcon(QStyle::SP_DialogApplyButton));
    wnd->setReadOnly(true);
    layout->addWidget(wnd);
    layout->addWidget(but);
    connect(but, SIGNAL(clicked()), this, SLOT(closeDlg()));

    QFile fl;
    fl.setFileName(":/" + type + ".html");
    fl.open(QIODevice::ReadOnly);
    QString str = QString::fromUtf8(fl.readAll());

    wnd->appendHtml(str);


}

void DialogHelp::closeDlg()
{
    this->accept();

}
