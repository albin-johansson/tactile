#include "tree_widget_utils.hpp"

#include "icons.hpp"

namespace tactile::gui {

auto make_tree_node(const QString& name, QTreeWidget* parent)
    -> QTreeWidgetItem*
{
  auto* item = new QTreeWidgetItem{parent};

  item->setFirstColumnSpanned(true);
  item->setIcon(0, icons::collapsed());
  item->setText(0, name);

  return item;
}

auto make_tree_item(const QString& name,
                    const QString& value,
                    QTreeWidgetItem* parent) -> QTreeWidgetItem*
{
  auto* item = new QTreeWidgetItem{parent};

  item->setText(0, name);
  item->setText(1, value);

  return item;
}

}  // namespace tactile::gui
