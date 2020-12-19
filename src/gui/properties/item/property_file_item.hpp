#pragma once

#include <QLineEdit>

#include "property_tree_item.hpp"

namespace tactile::gui {

class file_value_widget;

class property_file_item final : public property_tree_item
{
 public:
  property_file_item(const QString& name,
                     const core::property& property,
                     QTreeWidgetItem* parent);

  void enable_focus_view() override;

  void enable_idle_view() override;

  void set_value(const core::property& property) override;

  [[nodiscard]] auto property_type() const noexcept
      -> core::property::type override;

  [[nodiscard]] auto is_inline() const noexcept -> bool override;

 protected:
  [[nodiscard]] auto get_value_widget() -> QLineEdit* override;

 private:
  file_value_widget* m_valueWidget{};

  void spawn_dialog();
};

}  // namespace tactile::gui