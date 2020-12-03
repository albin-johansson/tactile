#pragma once

#include <QTreeWidgetItem>

#include "map_document.hpp"

namespace tactile::gui {

class map_tree_widget_item final : public QTreeWidgetItem
{
 public:
  explicit map_tree_widget_item(QTreeWidget* parent = nullptr);

  void update(const core::map_document& document);

 private:
  QTreeWidgetItem* m_rows{};
  QTreeWidgetItem* m_cols{};
  QTreeWidgetItem* m_tileWidth{};
  QTreeWidgetItem* m_tileHeight{};
};

}  // namespace tactile::gui
