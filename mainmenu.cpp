#include "mainmenu.h"
#include "textdata.h"

MainMenu::MainMenu(QWidget * par, TextData * dt):QMenuBar(par)
{
    QMenu * menu;

    menu = new QMenu("Файл");
    menu->addAction("Создать", [this]{emit createDocument();});
    menu->addAction("Открыть", [this]{emit loadDocument();});
    menu->addAction("Сохранить", [this]{emit saveDocument(false);});
    menu->addAction("Сохранить как", [this]{emit saveDocument(true);});
    menu->addAction("Закрыть", [this]{emit closeDocument();});
    addMenu(menu);

    menu = new QMenu("Важность");
    QPixmap ic(16, 16);
    foreach(auto el, dt->getSortListImportance()) {
        ic.fill(el->getColor());
        menu->addAction(ic, el->nameImportance, [this, el]{emit setImportance(el->getTag());});
    }
    addMenu(menu);

    menu = new QMenu("Скрытость");
    menu->addAction("скрыть", [this]{emit hideText(true);});
    menu->addAction("показать", [this]{emit hideText(false);});
    addMenu(menu);

    menu = new QMenu("Поиск");
    menu->addAction("по строке", [this]{emit searchString();});
    menu->addAction("по важности", [this]{emit searchImportance();});
    addMenu(menu);

    menu = new QMenu("Справка");
    menu->addAction("помощь", [this]{emit helpShow("help");});
    menu->addAction("о нас", [this]{emit helpShow("about");});
    addMenu(menu);
}
