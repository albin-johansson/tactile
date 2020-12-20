#pragma once

#include <QLineEdit>

#include "property_tree_item.hpp"

namespace tactile::gui {

class property_string_item final : public property_tree_item
{
 public:
  property_string_item(const QString& name,
                       const core::property& property,
                       QTreeWidgetItem* parent);

  void set_value(const core::property& property) override;

  [[nodiscard]] auto property_type() const noexcept
      -> core::property::type override;

 protected:
  [[nodiscard]] auto get_value_widget() -> QLineEdit* override;
};

}  // namespace tactile::gui
