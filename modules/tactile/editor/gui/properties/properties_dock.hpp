#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include "dialogs/add_property_dialog.hpp"
#include "dialogs/change_property_type_dialog.hpp"
#include "dialogs/rename_property_dialog.hpp"
#include "property_table.hpp"
#include "tactile_def.hpp"

namespace tactile {

class Model;

class PropertiesDock final {
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  void ShowAddPropertyDialog();

  void ShowRenamePropertyDialog(const std::string& name);

  void ShowChangePropertyTypeDialog(std::string name, attribute_type type);

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  PropertyTable mPropertyTable;
  AddPropertyDialog mAddDialog;
  RenamePropertyDialog mRenameDialog;
  ChangePropertyTypeDialog mChangeTypeDialog;
  bool mHasFocus{};
};

}  // namespace tactile
