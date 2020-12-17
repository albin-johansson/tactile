#include "tree_widget_item.hpp"

#include "icons.hpp"

namespace tactile::gui {

tree_widget_item::tree_widget_item(QTreeWidget* parent)
    : QTreeWidgetItem{parent}
{}

auto tree_widget_item::make_node(const QString& name, QTreeWidget* parent)
    -> tree_widget_item*
{
  auto* item = new tree_widget_item{parent};

  item->setFirstColumnSpanned(true);
  item->setIcon(0, icons::expanded());
  item->setText(0, name);
  item->setExpanded(true);

  return item;
}

void tree_widget_item::erase(QTreeWidgetItem* child)
{
#ifdef QT_DEBUG
  const auto index = indexOfChild(child);
  Q_ASSERT(index >= 0 && index < childCount());
#endif
  removeChild(child);  // Does not delete the child
  delete child;
}

}  // namespace tactile::gui
