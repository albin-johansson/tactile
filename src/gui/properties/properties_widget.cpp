#include "properties_widget.hpp"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

#include "add_property_dialog.hpp"
#include "icons.hpp"
#include "preferences.hpp"
#include "property_map_root_item.hpp"
#include "property_tree_item.hpp"
#include "tactile_qstring.hpp"
#include "tree_widget_utils.hpp"
#include "ui_properties_widget.h"

namespace tactile::gui {
namespace {

inline constexpr int nameColumn = 0;
inline constexpr int valueColumn = 1;

[[nodiscard]] auto value_string_from_type(const core::property::type type)
    -> QString
{
  switch (type) {
    case core::property::string:
    case core::property::file:
      return TACTILE_QSTRING(u"");

    case core::property::integer:
      return TACTILE_QSTRING(u"0");

    case core::property::floating:
      return TACTILE_QSTRING(u"0.0");

    case core::property::boolean:
      return TACTILE_QSTRING(u"false");

    case core::property::color:
      return TACTILE_QSTRING(u"COLOR");

    case core::property::object:
      return TACTILE_QSTRING(u"OBJECT");

    default:
      throw tactile_error{"Did not recognize property type for value string!"};
  }
}

[[nodiscard]] auto item_widget_for_type(const core::property::type type)
    -> QWidget*
{
  switch (type) {
    case core::property::string:
    case core::property::file:
    case core::property::object:
      [[fallthrough]];
    case core::property::color:
      return nullptr;
    case core::property::integer: {
      auto* edit = new QLineEdit{};
      edit->setFrame(false);
      edit->setValidator(new QIntValidator{});
      return edit;
    }
    case core::property::floating: {
      auto* edit = new QLineEdit{};
      edit->setFrame(false);
      edit->setValidator(new QDoubleValidator{});
      return edit;
    }
    case core::property::boolean: {
      auto* box = new QComboBox{};
      box->addItem(TACTILE_QSTRING(u"false"));
      box->addItem(TACTILE_QSTRING(u"true"));
      box->setCurrentIndex(0);
      box->setFrame(false);
      return box;
    }
    default:
      throw tactile_error{"Did not recognize property type for item widget!"};
  }
}

[[nodiscard]] auto tooltip_text_for_type(const core::property::type type)
    -> QString
{
  switch (type) {
    case core::property::string:
      return TACTILE_QSTRING(u"(string)");

    case core::property::file:
      return TACTILE_QSTRING(u"(file)");

    case core::property::object:
      return TACTILE_QSTRING(u"(object)");

    case core::property::color:
      return TACTILE_QSTRING(u"(color)");

    case core::property::integer:
      return TACTILE_QSTRING(u"(int)");

    case core::property::floating:
      return TACTILE_QSTRING(u"(float)");

    case core::property::boolean:
      return TACTILE_QSTRING(u"(bool)");

    default:
      throw tactile_error{"Did not recognize property type for tooltip text!"};
  }
}

}  // namespace

properties_widget::properties_widget(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::properties_widget{}}
{
  m_ui->setupUi(this);

  connect(m_ui->treeWidget,
          &QTreeWidget::itemCollapsed,
          [](QTreeWidgetItem* item) {
            item->setIcon(nameColumn, icons::collapsed());
          });

  connect(m_ui->treeWidget,
          &QTreeWidget::itemExpanded,
          [](QTreeWidgetItem* item) {
            item->setIcon(nameColumn, icons::expanded());
          });

  // clang-format off
  connect(m_ui->treeWidget, &QTreeWidget::itemChanged, this, &properties_widget::when_item_modified);
  connect(m_ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &properties_widget::when_item_double_clicked);
  connect(m_ui->treeWidget, &QTreeWidget::itemSelectionChanged, this, &properties_widget::when_item_selection_changed);
  connect(m_ui->newPropertyButton, &QPushButton::clicked, this, &properties_widget::when_new_property_button_clicked);
  connect(m_ui->removePropertyButton, &QPushButton::clicked, this, &properties_widget::when_remove_property_button_clicked);
  // clang-format on
}

void properties_widget::selected_map(const core::map_document& document)
{
  m_props.clear();
  m_ui->treeWidget->clear();

  auto* mapItem = new property_map_root_item{m_ui->treeWidget};
  mapItem->update(document);

  Q_ASSERT((m_predefinedRoot && m_predefinedRoot->parent()) ||
           !m_predefinedRoot);
  m_predefinedRoot = mapItem;

  Q_ASSERT((m_customRoot && m_customRoot->parent()) || !m_customRoot);
  m_customRoot = make_tree_node(TACTILE_QSTRING(u"Custom"), m_ui->treeWidget);

  m_ui->treeWidget->clearSelection();
  m_ui->newPropertyButton->setEnabled(false);
  m_ui->removePropertyButton->setEnabled(false);
  m_ui->duplicatePropertyButton->setEnabled(false);
  m_ui->downButton->setEnabled(false);
  m_ui->upButton->setEnabled(false);
}

void properties_widget::updated_map(const core::map_document& document)
{}

void properties_widget::select_layer(const core::layer& layer)
{}

void properties_widget::added_property(const QString& name,
                                       const core::property& property)
{}

void properties_widget::updated_property(const QString& name,
                                         const core::property& property)
{}

void properties_widget::removed_property(const QString& name)
{}

void properties_widget::add_item(const QString& name,
                                 const core::property::type type)
{
  if (auto* item = m_ui->treeWidget->currentItem()) {
    auto* property =
        new property_tree_item{name, value_string_from_type(type), item};
    property->setToolTip(0, tooltip_text_for_type(type));

    if (auto* widget = item_widget_for_type(type)) {
      m_ui->treeWidget->setItemWidget(property, 1, widget);
    }

    when_item_double_clicked(property, 1);
  }
}

void properties_widget::when_new_property_button_clicked()
{
  add_property_dialog::spawn(
      [this](const QString& name, const core::property::type type) {
        add_item(name, type);
      },
      m_ui->treeWidget,
      this);
}

void properties_widget::when_remove_property_button_clicked()
{
  // This looks dodgy but this is actually the way to remove items
  for (auto* item : m_ui->treeWidget->selectedItems()) {
    delete item;
  }
}

void properties_widget::when_item_selection_changed()
{
  const auto* item = m_ui->treeWidget->currentItem();
  Q_ASSERT(item);

  m_ui->newPropertyButton->setEnabled(item == m_customRoot);
  m_ui->removePropertyButton->setEnabled(item->parent() == m_customRoot);
}

void properties_widget::when_item_modified(QTreeWidgetItem* item,
                                           const int column)
{}

void properties_widget::when_item_double_clicked(QTreeWidgetItem* item,
                                                 const int column)
{
  if (auto* treeItem = dynamic_cast<property_tree_item*>(item)) {
    if (column == 0 && !treeItem->is_name_editable()) {
      return;  // Can't change name of predefined properties
    } else {
      const auto flags = treeItem->flags();
      treeItem->setFlags(flags | Qt::ItemIsEditable);

      m_ui->treeWidget->editItem(treeItem, column);
      treeItem->setFlags(flags);
    }
  }
}

}  // namespace tactile::gui
