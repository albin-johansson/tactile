#pragma once

#include "property_tree_item.hpp"

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

}  // namespace tactile::gui
