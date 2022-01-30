#pragma once

#include "core/components/component.hpp"
#include "dialogs/create_component_attribute_dialog.hpp"
#include "dialogs/create_component_dialog.hpp"
#include "dialogs/rename_component_attribute_dialog.hpp"
#include "dialogs/rename_component_dialog.hpp"
#include "editor/gui/dialogs/dialog.hpp"
#include "tactile_def.hpp"

namespace tactile {

class ComponentEditor final : public ADialog {
 public:
  ComponentEditor();

  void Open(const Model& model);

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

 private:
  CreateComponentDialog mCreateComponentDialog;
  CreateComponentAttributeDialog mCreateComponentAttributeDialog;
  RenameComponentDialog mRenameComponentDialog;
  RenameComponentAttributeDialog mRenameComponentAttributeDialog;
  maybe<component_id> mActiveComponent;

  void ShowComponentComboPopup(const entt::registry& registry,
                               entt::dispatcher& dispatcher);

  void ShowComponentAttributes(const entt::registry& registry,
                               entt::dispatcher& dispatcher,
                               component_id id);

  void ShowComponentAttribute(entt::dispatcher& dispatcher,
                              component_id id,
                              const std::string& name,
                              const attribute_value& value);
};

}  // namespace tactile
