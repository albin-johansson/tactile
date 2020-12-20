#include "property_tree_item.hpp"

namespace tactile::gui {

property_tree_item::property_tree_item(const QString& name,
                                       QTreeWidgetItem* parent)
    : tree_widget_item{parent}
{
  Q_ASSERT(treeWidget());
  Q_ASSERT(parent);
  setText(0, name);
}

void property_tree_item::set_name_editable(const bool editable)
{
  Q_ASSERT(treeWidget());
  m_isValueEditable = editable;

  auto newFlags = flags();
  newFlags.setFlag(Qt::ItemIsEditable, editable);
  setFlags(newFlags);
}

void property_tree_item::set_value_editable(const bool editable)
{
  m_isValueEditable = editable;
  if (auto* widget = get_value_widget()) {
    widget->setEnabled(m_isValueEditable);
  }
}

auto property_tree_item::is_name_editable() const noexcept -> bool
{
  return m_isNameEditable;
}

auto property_tree_item::is_value_editable() const noexcept -> bool
{
  return m_isValueEditable;
}

}  // namespace tactile::gui
