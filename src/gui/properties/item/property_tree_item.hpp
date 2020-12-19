#pragma once

#include "algorithm.hpp"
#include "property.hpp"
#include "tactile_error.hpp"
#include "tree_widget_item.hpp"

namespace tactile::gui {

// TODO all items should provide two "states": "idle view" and "when focused"

class property_tree_item : public tree_widget_item
{
 public:
  void set_name_editable(bool editable);

  void set_value_editable(bool editable);

  // clang-format off

  virtual void set_value(const core::property& property) = 0;

  [[nodiscard]]
  virtual auto property_type() const noexcept -> core::property::type = 0;

  [[nodiscard]]
  virtual auto get_value_widget() -> QWidget* = 0;

  [[nodiscard]]
  virtual auto is_inline() const noexcept -> bool = 0;

  [[nodiscard]]
  auto is_name_editable() const noexcept -> bool;

  [[nodiscard]]
  auto is_value_editable() const noexcept -> bool;

  // clang-format on

 protected:
  using tree_widget_item::tree_widget_item;

 private:
  bool m_isNameEditable{true};
  bool m_isValueEditable{true};
};

}  // namespace tactile::gui
