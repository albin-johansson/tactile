#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include "dialogs/rename_property_dialog.hpp"
#include "property_table.hpp"

namespace Tactile {

class PropertiesDock final {
 public:
  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

  void ShowRenamePropertyDialog(const std::string& name);

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  PropertyTable mPropertyTable;
  RenamePropertyDialog mRenameDialog;
  bool mHasFocus{};
};

}  // namespace Tactile
