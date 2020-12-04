#include "tree_widget_utils.hpp"

#include "icons.hpp"
#include "property_tree_item.hpp"

namespace tactile::gui {

auto make_tree_node(const QString& name, QTreeWidget* parent)
    -> QTreeWidgetItem*
{
  auto* item = new QTreeWidgetItem{parent};

  item->setFirstColumnSpanned(true);
  item->setIcon(0, icons::expanded());
  item->setText(0, name);
  item->setExpanded(true);

  return item;
}

}  // namespace tactile::gui
