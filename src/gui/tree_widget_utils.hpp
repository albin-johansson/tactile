#pragma once

#include <QTreeWidgetItem>

#include "algorithm.hpp"
#include "tactile_error.hpp"

namespace tactile::gui {

/**
 * \brief Creates and returns a tree widget item that acts as a root node.
 *
 * \param name The name of the tree widget node.
 * \param parent a pointer to the parent tree widget. If null, the returned item
 * must be manually deleted.
 *
 * \return a pointer to the created tree widget item.
 *
 * \since 0.2.0
 */
auto make_tree_node(const QString& name, QTreeWidget* parent = nullptr)
    -> QTreeWidgetItem*;

/**
 * \brief Creates and returns a tree widget item.
 *
 * \param name the name of the tree widget item.
 * \param value the value that will be displayed in the item.
 * \param parent a pointer to the parent tree widget item. If null, the returned
 * item must be manually deleted.
 *
 * \return a pointer to the created tree widget item.
 *
 * \since 0.2.0
 */
auto make_tree_item(const QString& name,
                    const QString& value,
                    QTreeWidgetItem* parent = nullptr) -> QTreeWidgetItem*;

/**
 * \copydoc make_tree_item()
 *
 * \tparam T the type of the arithmetic value.
 *
 * \throws tactile_error if the value cannot be converted to a string.
 */
template <arithmetic T>
auto make_tree_item(const QString& name,
                    T value,
                    QTreeWidgetItem* parent = nullptr) -> QTreeWidgetItem*
{
  const auto str = QString::number(value);
  if (str.isNull()) {
    throw tactile_error{"Could not create tree item with arithmetic value!"};
  }
  return make_tree_item(name, str, parent);
}

}  // namespace tactile::gui
