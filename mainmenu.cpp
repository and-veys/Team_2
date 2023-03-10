#include "mainmenu.h"
#include "textdata.h"

#include <QCommonStyle>
#include <QKeyEvent>


MainMenu::MainMenu(QWidget * par, TextData * dt):QMenuBar(par)
{
    QMenu * menu;
    QCommonStyle style;
    auto getIcon = [&style](QStyle::StandardPixmap si){return style.standardIcon(si);};
    auto addHotKey = [this](QAction * act, Qt::KeyboardModifiers mod, int key){
          hotKey.insert(key, new HotKey(act, mod, key));
    };

    menu = new QMenu("Файл");
    addHotKey(
        menu->addAction(getIcon(QStyle::SP_FileIcon), "Создать (CTRL+N)", [this]{emit createDocument();}),
        Qt::ControlModifier, Qt::Key_N);
    addHotKey(
         menu->addAction(getIcon(QStyle::SP_DialogOpenButton), "Открыть (CTRL+O)", [this]{emit loadDocument();}),
         Qt::ControlModifier, Qt::Key_O);
    addHotKey(
         menu->addAction(getIcon(QStyle::SP_DialogSaveButton), "Сохранить (CTRL+A)", [this]{emit saveDocument(false);}),
         Qt::ControlModifier, Qt::Key_A);
    addHotKey(
        menu->addAction(getIcon(QStyle::SP_DialogSaveButton), "Сохранить как (CTRL+B)", [this]{emit saveDocument(true);}),
         Qt::ControlModifier, Qt::Key_B);
    addHotKey(
        menu->addAction(getIcon(QStyle::SP_DialogCloseButton), "Закрыть (CTRL+C)", [this]{emit closeDocument();}),
         Qt::ControlModifier, Qt::Key_C);
    addMenu(menu);

    menu = new QMenu("Важность");
    QPixmap ic(16, 16);
    int i = 0;
    foreach(auto el, dt->getSortListImportance()) {
        ic.fill(el->getColor());
        addHotKey(
            menu->addAction(ic, el->getNameImportance() + " (CTRL+" + QString::number(i) + ")", [this, el]{emit setImportance(el->getTag());}),
            Qt::ControlModifier, Qt::Key_0 + i);
        ++i;
    }
    addMenu(menu);

    menu = new QMenu("Скрытость");
    addHotKey(
        menu->addAction(getIcon(QStyle::SP_DirClosedIcon), "Скрыть (CTRL+H)", [this]{emit hideText(true);}),
         Qt::ControlModifier, Qt::Key_H);
    addHotKey(
        menu->addAction(getIcon(QStyle::SP_DirOpenIcon), "Показать (CTRL+S)", [this]{emit hideText(false);}),
        Qt::ControlModifier, Qt::Key_S);
    addMenu(menu);

    menu = new QMenu("Поиск");
    addHotKey(
        menu->addAction(getIcon(QStyle::SP_FileDialogContentsView),"По строке (CTRL+R)", [this]{emit searchString();}),
        Qt::ControlModifier, Qt::Key_R);
    addHotKey(
        menu->addAction(getIcon(QStyle::SP_MessageBoxWarning),"По важности (CTRL+I)", [this]{emit searchImportance();}),
        Qt::ControlModifier, Qt::Key_I);

    addHotKey(
        menu->addAction(getIcon(QStyle::SP_MediaPause),"По спрятанному (CTRL+T)", [this]{emit searchHide();}),
        Qt::ControlModifier, Qt::Key_T);
    addMenu(menu);

    menu = new QMenu("Справка");
    addHotKey(
        menu->addAction(getIcon(QStyle::SP_MessageBoxQuestion), "Помощь (CTRL+W)", [this]{emit helpShow("help");}),
        Qt::ControlModifier, Qt::Key_W);
    addHotKey(
        menu->addAction(getIcon(QStyle::SP_MessageBoxInformation), "О нас (CTRL+U)", [this]{emit helpShow("about");}),
        Qt::ControlModifier, Qt::Key_U);
    addMenu(menu);
}

MainMenu::~MainMenu()
{
    foreach(auto el, hotKey)
        delete el;
}

bool MainMenu::isHotKey(QKeyEvent *event)
{
    int key = event->key();
    if(!hotKey.contains(key)) return false;
    if(!hotKey.value(key)->isHotKey(event->modifiers())) return false;
    return true;
}

HotKey::HotKey(QAction *act, Qt::KeyboardModifiers mod, int k)
{
    action.insert(mod, act);
    key = k;
}

bool HotKey::isHotKey(Qt::KeyboardModifiers mod)
{
    if(!action.contains(mod)) return false;
    action.value(mod)->trigger();
    return true;

}
