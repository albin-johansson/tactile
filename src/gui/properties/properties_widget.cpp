#include "properties_widget.hpp"

#include "add_property_dialog.hpp"
#include "icons.hpp"
#include "property_map_root_item.hpp"
#include "property_tree_item.hpp"
#include "tactile_qstring.hpp"
#include "tree_widget_utils.hpp"
#include "ui_properties_widget.h"

namespace tactile::gui {
namespace {

inline constexpr int nameColumn = 0;
inline constexpr int valueColumn = 1;

}  // namespace

properties_widget::properties_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{new Ui::properties_widget{}}
{
  m_ui->setupUi(this);

  connect(m_ui->treeWidget,
          &QTreeWidget::itemCollapsed,
          [](QTreeWidgetItem* item) {
            if (!item->parent()) {
              item->setIcon(nameColumn, icons::collapsed());
            }
          });

  connect(m_ui->treeWidget,
          &QTreeWidget::itemExpanded,
          [](QTreeWidgetItem* item) {
            if (!item->parent()) {
              item->setIcon(nameColumn, icons::expanded());
            }
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
  m_ui->removePropertyButton->setEnabled(false);
  m_ui->duplicatePropertyButton->setEnabled(false);
  m_ui->downButton->setEnabled(false);
  m_ui->upButton->setEnabled(false);
}

void properties_widget::updated_map(const core::map_document& document)
{}

void properties_widget::select_layer(const core::tile_layer& layer)
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
  if (auto* item = m_customRoot) {
    auto* property = new property_tree_item{name, type, item};
    property->set_name_editable(true);
    if (property->is_inline_property()) {
      when_item_double_clicked(property, valueColumn);
    }
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
  for (auto* item : m_ui->treeWidget->selectedItems()) {
    const auto index = m_customRoot->indexOfChild(item);
    Q_ASSERT(index >= 0 && index < m_customRoot->childCount());
    delete m_customRoot->takeChild(index);
  }
}

void properties_widget::when_item_selection_changed()
{
  if (const auto* item = m_ui->treeWidget->currentItem()) {
    m_ui->removePropertyButton->setEnabled(item->parent() == m_customRoot);
  }
}

void properties_widget::when_item_modified(QTreeWidgetItem* item,
                                           const int column)
{}

void properties_widget::when_item_double_clicked(QTreeWidgetItem* item,
                                                 const int column)
{
  if (auto* treeItem = dynamic_cast<property_tree_item*>(item)) {
    if (column == nameColumn && !treeItem->is_name_editable()) {
      return;  // Can't change name of predefined properties
    } else if (treeItem->flags() & Qt::ItemIsEditable) {
      m_ui->treeWidget->editItem(treeItem, column);
    }
  }
}

}  // namespace tactile::gui
