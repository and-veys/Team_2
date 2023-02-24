#include "mainmenu.h"
#include "textdata.h"

#include <QCommonStyle>

MainMenu::MainMenu(QWidget * par, TextData * dt):QMenuBar(par)
{
    QMenu * menu;
    QCommonStyle style;
    auto getIcon = [&style](QStyle::StandardPixmap si){return style.standardIcon(si);};

    menu = new QMenu("Файл");
    menu->addAction(getIcon(QStyle::SP_FileIcon), "Создать", [this]{emit createDocument();});
    menu->addAction(getIcon(QStyle::SP_DialogOpenButton), "Открыть", [this]{emit loadDocument();});
    menu->addAction(getIcon(QStyle::SP_DialogSaveButton), "Сохранить", [this]{emit saveDocument(false);});
    menu->addAction(getIcon(QStyle::SP_DialogSaveButton), "Сохранить как", [this]{emit saveDocument(true);});
    menu->addAction(getIcon(QStyle::SP_DialogCloseButton), "Закрыть", [this]{emit closeDocument();});
    addMenu(menu);

    menu = new QMenu("Важность");
    QPixmap ic(16, 16);
    foreach(auto el, dt->getSortListImportance()) {
        ic.fill(el->getColor());
        menu->addAction(ic, el->getNameImportance(), [this, el]{emit setImportance(el->getTag());});
    }
    addMenu(menu);

    menu = new QMenu("Скрытость");
    menu->addAction(getIcon(QStyle::SP_DirClosedIcon), "Скрыть", [this]{emit hideText(true);});
    menu->addAction(getIcon(QStyle::SP_DirOpenIcon), "Показать", [this]{emit hideText(false);});
    addMenu(menu);

    menu = new QMenu("Поиск");
    menu->addAction(getIcon(QStyle::SP_FileDialogContentsView),"По строке", [this]{emit searchString();});
    menu->addAction(getIcon(QStyle::SP_MessageBoxWarning),"По важности", [this]{emit searchImportance();});
    addMenu(menu);

    menu = new QMenu("Справка");
    menu->addAction(getIcon(QStyle::SP_MessageBoxQuestion), "Помощь", [this]{emit helpShow("help");});
    menu->addAction(getIcon(QStyle::SP_MessageBoxInformation), "О нас", [this]{emit helpShow("about");});
    addMenu(menu);

    /***************************************/
       menu = new QMenu("Тест");
       menu->addAction(getIcon(QStyle::SP_MessageBoxQuestion), "Тест", [this]{emit signalTest();});
       addMenu(menu);

}
