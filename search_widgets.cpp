#include "search_widgets.h"
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>



/**
 * @brief AbstractSearchWidget::AbstractSearchWidget
 * @param label
 * @param parent
 * @param f
 */
AbstractSearchWidget::AbstractSearchWidget(const QString& label, QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setWindowTitle(label);
    resize(300, 100);
    layout = new QGridLayout(this);
    prev = new QPushButton(QString("<"), this);
    next = new QPushButton(QString(">"), this);

    connect(prev, &QPushButton::clicked, this, [this](){ emit searchPrev(searcheble_text);});
    connect(next, &QPushButton::clicked, this, [this](){ emit searchNext(searcheble_text);});

}


/**
 * @brief SearchWidgetImportance::SearchWidgetImportance
 * @param label
 * @param parent
 * @param f
 */
SearchWidgetImportance::SearchWidgetImportance(const QString &label, const TextData& td, QWidget *parent, Qt::WindowFlags f)
    : AbstractSearchWidget(label, parent, f), td(td)
{
    create_data_widget();
}


/**
 * @brief SearchWidgetImportance::create_data_widget
 */
void SearchWidgetImportance::create_data_widget()
{

   data_widget = new QComboBox(this);
   auto data_combobox = dynamic_cast<QComboBox*>(data_widget);

   auto importances = td.getSortListImportance();
   size_t i = 0;
   for(auto & im: importances) {
       data_combobox->insertItem(i, im->nameImportance, im->getTag());
   }

   layout->addWidget(data_widget);

   auto buttons_layout = new QHBoxLayout(this);
   layout->addItem(buttons_layout);
   buttons_layout->addWidget(prev);
   buttons_layout->addWidget(next);

   connect(data_combobox, &QComboBox::currentIndexChanged, this, [this, data_combobox](){ searcheble_text = data_combobox->currentData().toString();});

   show();
}


/**
 * @brief SearchWidgetString::SearchWidgetString
 * @param label
 * @param parent
 * @param f
 */
SearchWidgetString::SearchWidgetString(const QString &label, QWidget *parent, Qt::WindowFlags f)
    : AbstractSearchWidget(label, parent, f)
{
    create_data_widget();
}

/**
 * @brief SearchWidgetString::create_data_widget
 */
void SearchWidgetString::create_data_widget()
{
    data_widget = new QLineEdit(this);
    auto data_lineedit = dynamic_cast<QLineEdit*>(data_widget);
    layout->addWidget(data_widget);

    auto buttons_layout = new QHBoxLayout(this);
    layout->addItem(buttons_layout);
    buttons_layout->addWidget(prev);
    buttons_layout->addWidget(next);

    connect(data_lineedit, &QLineEdit::textChanged, this, [this, data_lineedit](){ searcheble_text = data_lineedit->text();});

    show();
}


