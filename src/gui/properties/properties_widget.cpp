#include "properties_widget.hpp"

#include "add_property_dialog.hpp"
#include "properties_context_menu.hpp"
#include "property_map_root_item.hpp"
#include "property_tree_item.hpp"
#include "tactile_qstring.hpp"
#include "tree_widget_utils.hpp"
#include "ui_properties_widget.h"
#include "property_item_factory.hpp"

namespace tactile::gui {

properties_widget::properties_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{new Ui::properties_widget{}}
    , m_contextMenu{new properties_context_menu{this}}
{
  m_ui->setupUi(this);

  setup_expand_collapse_icons(m_ui->treeWidget);
  m_ui->treeWidget->header()->setSectionResizeMode(
      QHeaderView::ResizeToContents);

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
    when_item_double_clicked(m_ui->treeWidget->currentItem(), 0);
  });
  // clang-format on

  update_actions();
}

void properties_widget::selected_map(const core::map_document& document)
{
  m_ui->treeWidget->clear();

  auto* mapItem = new property_map_root_item{m_ui->treeWidget};
  mapItem->update(document);

  m_predefinedRoot = mapItem;

  Q_ASSERT((m_customRoot && m_customRoot->parent()) || !m_customRoot);
  m_customRoot =
      tree_widget_item::make_node(TACTILE_QSTRING(u"Custom"), m_ui->treeWidget);

  m_ui->treeWidget->clearSelection();
  update_actions();
}

void properties_widget::select_layer(const core::tile_layer& layer)
{
  // TODO
  update_actions();
}

void properties_widget::added_property(const QString& name,
                                       const core::property& property)
{
  Q_ASSERT(m_customRoot);
  auto* item = property_item_factory::make(name, property, m_customRoot);
  item->set_name_editable(true);
  if (item->is_inline()) {
    when_item_double_clicked(item, 1);
  }
  update_actions();
}

void properties_widget::removed_property(const QString& name)
{
  auto* current = m_ui->treeWidget->currentItem();
  Q_ASSERT(current);
  Q_ASSERT(current->parent() == m_customRoot);
  Q_ASSERT(current->text(0) == name);
  m_customRoot->erase(current);
  update_actions();
}

void properties_widget::moved_property_up(const QString& name)
{}

void properties_widget::moved_property_down(const QString& name)
{}

void properties_widget::duplicated_property(const QString& name)
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

void properties_widget::trigger_context_menu(const QPoint& pos)
{
  m_contextMenu->exec(mapToGlobal(pos));
}

void properties_widget::when_new_property_button_clicked()
{
  add_property_dialog::spawn(
      [this](const QString& name, const core::property::type type) {
        emit request_add_property(name, type);
      },
      m_ui->treeWidget,
      this);
}

void properties_widget::when_remove_property_button_clicked()
{
  if (auto* current = m_ui->treeWidget->currentItem()) {
    emit request_remove_property(current->text(0));
  }
}

void properties_widget::when_item_selection_changed()
{
  update_actions();
}

void properties_widget::when_item_modified(QTreeWidgetItem* item,
                                           const int column)
{
  if (column == 0 && m_cachedName) {
    emit has_renamed_property(*m_cachedName, item->text(0));
    m_cachedName.reset();
  } else if (column == 1 && item->text(column).isEmpty()) {
    item->setText(column, TACTILE_QSTRING(u"N/A"));
  }
}

void properties_widget::when_item_double_clicked(QTreeWidgetItem* item,
                                                 const int column)
{
  if (auto* treeItem = dynamic_cast<property_tree_item*>(item)) {
    if (column == 0 && !treeItem->is_name_editable()) {
      return;
    } else if (!treeItem->isDisabled()) {
      if (column == 0) {
        m_cachedName.emplace(item->text(0));
      } else {
        m_cachedName.reset();
      }

      m_ui->treeWidget->editItem(treeItem, column);
    }
  }
}

}  // namespace tactile::gui
