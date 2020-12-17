#include "properties_widget.hpp"

#include "add_property_dialog.hpp"
#include "properties_context_menu.hpp"
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
    , m_contextMenu{new properties_context_menu{this}}
{
  m_ui->setupUi(this);

  setup_expand_collapse_icons(m_ui->treeWidget);

  // clang-format off
  connect(m_ui->treeWidget, &QWidget::customContextMenuRequested, this, &properties_widget::trigger_context_menu);
  connect(m_ui->treeWidget, &QTreeWidget::itemChanged, this, &properties_widget::when_item_modified);
  connect(m_ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &properties_widget::when_item_double_clicked);
  connect(m_ui->treeWidget, &QTreeWidget::itemSelectionChanged, this, &properties_widget::when_item_selection_changed);
  connect(m_ui->addButton, &QPushButton::clicked, this, &properties_widget::when_new_property_button_clicked);
  connect(m_ui->removeButton, &QPushButton::clicked, this, &properties_widget::when_remove_property_button_clicked);

  connect(m_contextMenu, &properties_context_menu::add, this, &properties_widget::when_new_property_button_clicked);
  connect(m_contextMenu, &properties_context_menu::remove, this, &properties_widget::when_remove_property_button_clicked);
  connect(m_contextMenu, &properties_context_menu::rename, [this] {
    Q_ASSERT(m_ui->treeWidget->currentItem());
    m_ui->treeWidget->editItem(m_ui->treeWidget->currentItem(), 0);
  });
  // clang-format on

  update_actions();
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
  m_customRoot =
      tree_widget_item::make_node(TACTILE_QSTRING(u"Custom"), m_ui->treeWidget);

  m_ui->treeWidget->clearSelection();
  update_actions();
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
void properties_widget::update_actions()
{
  auto* item =
      dynamic_cast<property_tree_item*>(m_ui->treeWidget->currentItem());
  const auto isEditable = item && item->parent() == m_customRoot;

  if (isEditable) {
    const auto* parent = item->parent();
    const auto index = parent->indexOfChild(item);
    m_ui->upButton->setEnabled(index != 0);
    m_ui->downButton->setEnabled(index != parent->childCount() - 1);
  } else {
    m_ui->upButton->setEnabled(false);
    m_ui->downButton->setEnabled(false);
  }

  m_ui->duplicateButton->setEnabled(isEditable);
  m_ui->renameButton->setEnabled(isEditable);
  m_ui->removeButton->setEnabled(isEditable);

  m_contextMenu->set_duplicate_enabled(m_ui->duplicateButton->isEnabled());
  m_contextMenu->set_rename_enabled(m_ui->duplicateButton->isEnabled());
  m_contextMenu->set_remove_enabled(m_ui->removeButton->isEnabled());
  m_contextMenu->set_up_enabled(m_ui->upButton->isEnabled());
  m_contextMenu->set_down_enabled(m_ui->downButton->isEnabled());
  m_contextMenu->set_change_type_enabled(isEditable);
  m_contextMenu->set_copy_enabled(isEditable);
  m_contextMenu->set_paste_enabled(isEditable);
  if (item) {
    m_contextMenu->set_current_type(item->property_type());
  }
}

void properties_widget::add_item(const QString& name,
                                 const core::property::type type)
{
  Q_ASSERT(m_customRoot);
  auto* property = new property_tree_item{name, type, m_customRoot};
  property->set_name_editable(true);
  if (property->is_inline_property()) {
    when_item_double_clicked(property, 1);
  }
}

void properties_widget::trigger_context_menu(const QPoint& pos)
{
  m_contextMenu->exec(mapToGlobal(pos));
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
  if (auto* current = m_ui->treeWidget->currentItem()) {
    Q_ASSERT(current->parent() == m_customRoot);
    m_customRoot->erase(current);
  }
}

void properties_widget::when_item_selection_changed()
{
  update_actions();
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
