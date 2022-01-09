#pragma once

#include <tactile_def.hpp>

#include "core/components/component.hpp"
#include "create_component_attribute_dialog.hpp"
#include "create_component_dialog.hpp"
#include "editor/gui/dialogs/dialog.hpp"
#include "rename_component_attribute_dialog.hpp"
#include "rename_component_dialog.hpp"

namespace Tactile {

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
  Maybe<ComponentID> mActiveComponent;

  void ShowComponentComboPopup(const entt::registry& registry,
                               entt::dispatcher& dispatcher);

  void ShowComponentAttributes(const entt::registry& registry,
                               entt::dispatcher& dispatcher,
                               ComponentID id);

  void ShowComponentAttribute(entt::dispatcher& dispatcher,
                              ComponentID id,
                              const std::string& name,
                              const PropertyValue& value);
};

}  // namespace Tactile