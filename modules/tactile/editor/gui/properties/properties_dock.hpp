#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include <tactile_def.hpp>

#include "dialogs/add_property_dialog.hpp"
#include "dialogs/rename_property_dialog.hpp"
#include "dialogs/change_property_type_dialog.hpp"
#include "property_table.hpp"

namespace Tactile {

class PropertiesDock final {
 public:
  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

  void ShowAddPropertyDialog();

  void ShowRenamePropertyDialog(const std::string& name);

  void ShowChangePropertyTypeDialog(std::string name, PropertyType type);

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  PropertyTable mPropertyTable;
  AddPropertyDialog mAddDialog;
  RenamePropertyDialog mRenameDialog;
  ChangePropertyTypeDialog mChangeTypeDialog;
  bool mHasFocus{};
};

}  // namespace Tactile
