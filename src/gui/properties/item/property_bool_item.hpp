#pragma once

#include <QCheckBox>

#include "property_tree_item.hpp"

namespace tactile::gui {

class property_bool_item final : public property_tree_item
{
 public:
  property_bool_item(const QString& name,
                     const core::property& property,
                     QTreeWidgetItem* parent);

  void set_value(const core::property& property) override;

  [[nodiscard]] auto property_type() const noexcept
      -> core::property::type override;

  [[nodiscard]] auto is_inline() const noexcept -> bool override;

 protected:
  [[nodiscard]] auto get_value_widget() -> QCheckBox* override;
};

}  // namespace tactile::gui
