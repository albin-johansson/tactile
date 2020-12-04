#include "property_tree_item.hpp"

namespace tactile::gui {

property_tree_item::property_tree_item(QTreeWidgetItem* parent)
    : QTreeWidgetItem{parent}
{
  
}

property_tree_item::property_tree_item(const QString& name,
                                       const QString& value,
                                       QTreeWidgetItem* parent)
    : property_tree_item{parent}
{
  setText(0, name);
  setText(1, value);
}

void property_tree_item::set_name_editable(const bool editable) noexcept
{
  m_nameEditable = editable;
}

auto property_tree_item::is_name_editable() const noexcept -> bool
{
  return m_nameEditable;
}

}  // namespace tactile::gui
