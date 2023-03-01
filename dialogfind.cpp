#include "dialogfind.h"

#include <QComboBox>
#include <QCommonStyle>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

DialogFind::DialogFind(const QString & title, QWidget *parent):QDialog(parent)
{
    QGridLayout * layout = new QGridLayout(this);
    QPushButton * but_prev = new QPushButton(this);
    QPushButton * but_next = new QPushButton(this);
    QPushButton * but_exit = new QPushButton(this);

    QCommonStyle style;
    but_prev->setIcon(style.standardIcon(QStyle::SP_ArrowUp));
    but_next->setIcon(style.standardIcon(QStyle::SP_ArrowDown));
    but_exit->setIcon(style.standardIcon(QStyle::SP_DialogApplyButton));

    connect(but_next, SIGNAL(clicked()), this, SLOT(next()));
    connect(but_prev, SIGNAL(clicked()), this, SLOT(prev()));
    connect(but_exit, SIGNAL(clicked()), this, SLOT(closeDlg()));

    layout->addWidget(but_prev, 1, 1);
    layout->addWidget(but_next, 1, 2);
    layout->addWidget(but_exit, 1, 3);
    setWindowTitle(title);
    resize(300, 150);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void DialogFind::setMainWidget(QWidget *wnd)
{
    QGridLayout * layout = qobject_cast<QGridLayout *>(this->layout());
    layout->addWidget(wnd, 0, 1, 1, 3);
}

DialogFindString::DialogFindString(const QString &title, QWidget *parent) : DialogFind(title, parent)
{
    QLineEdit * line = new QLineEdit(this);
    setMainWidget(line);
    connect(line, &QLineEdit::textChanged, this, &DialogFindString::setInformation);
    searchData = "";
}

DialogFindHide::DialogFindHide(ParameterHide *hd, const QString &title, QWidget *parent) : DialogFind(title, parent)
{
    searchData = hd->getTag();
}

DialogFindImportance::DialogFindImportance(QList<ParameterImportance *> imp, const QString &title, QWidget *parent) : DialogFind(title, parent)
{
    combo = new QComboBox(this);
    setMainWidget(combo);
    QPixmap ic(16, 16);
    foreach(auto el, imp) {
        ic.fill(el->getColor());
        combo->addItem(ic, el->getNameImportance(), el->getTag());
    }
    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DialogFindImportance::setInformation);
    searchData = imp.at(0)->getTag();
}
