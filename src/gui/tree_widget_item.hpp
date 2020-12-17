#pragma once

#include <QTreeWidgetItem>

namespace tactile::gui {

class tree_widget_item final : public QTreeWidgetItem
{
 public:
  explicit tree_widget_item(QTreeWidget* parent);

  /**
   * \brief Creates and returns a tree widget item that acts as a root node.
   *
   * \param name The name of the tree widget node.
   * \param parent a pointer to the parent tree widget. If null, the returned
   * item must be manually deleted.
   *
   * \return a pointer to the created tree widget item.
   *
   * \since 0.2.0
   */
  static auto make_node(const QString& name, QTreeWidget* parent)
      -> tree_widget_item*;

  /**
   * \brief Removes a child from the tree widget item.
   *
   * \pre `child` must be a child item of the invoked tree widget item.
   *
   * \note The item _will be deleted_.
   *
   * \param child a pointer to the item that will be removed and deleted.
   *
   * \since 0.2.0
   */
  void erase(QTreeWidgetItem* child);
};

}  // namespace tactile::gui
