#pragma once

#include "property_tree_item.hpp"

namespace tactile::gui {

class property_item_factory final
{
 public:
  property_item_factory() = delete;

  [[nodiscard]] static auto make(const QString& name,
                                 const core::property& property,
                                 QTreeWidgetItem* parent)
      -> property_tree_item*;
};

}  // namespace tactile::gui
