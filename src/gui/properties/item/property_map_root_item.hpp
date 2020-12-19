#pragma once

#include <QTreeWidgetItem>

#include "map_document.hpp"
#include "property_tree_item.hpp"

namespace tactile::gui {

class property_map_root_item final : public QTreeWidgetItem
{
 public:
  explicit property_map_root_item(QTreeWidget* parent = nullptr);

  void update(const core::map_document& document);

 private:
  property_tree_item* m_rows{};
  property_tree_item* m_cols{};
  property_tree_item* m_tileWidth{};
  property_tree_item* m_tileHeight{};
};

}  // namespace tactile::gui
