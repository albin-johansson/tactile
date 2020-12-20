#pragma once

#include "property_tree_item.hpp"

namespace tactile::gui {

class property_object_item final : public property_tree_item
{
 public:
  property_object_item(const QString& name,
                       const core::property& property,
                       QTreeWidgetItem* parent);

  void set_value(const core::property& property) override;

  [[nodiscard]] auto property_type() const noexcept
      -> core::property::type override;

  [[nodiscard]] auto get_value_widget() -> QWidget* override;
};

}  // namespace tactile::gui
