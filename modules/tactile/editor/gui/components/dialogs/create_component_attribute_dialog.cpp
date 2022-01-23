#include "create_component_attribute_dialog.hpp"

#include <string>  // string

#include "core/systems/component_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/model.hpp"

namespace tactile {

CreateComponentAttributeDialog::CreateComponentAttributeDialog()
    : AStringInputDialog{"Create Attribute"}
{
  SetAcceptButtonLabel("Create");
  SetInputHint("Attribute name");
}

void CreateComponentAttributeDialog::Open(const ComponentID id)
{
  mComponentId = id;
  Show("");
}

auto CreateComponentAttributeDialog::Validate(const Model& model,
                                              const std::string_view input) const -> bool
{
  const auto& registry = model.GetActiveRegistryRef();
  return !input.empty() &&
         !sys::IsComponentAttributeNameTaken(registry, mComponentId, input);
}

void CreateComponentAttributeDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CreateComponentAttributeEvent>(mComponentId,
                                                    std::string{GetCurrentInput()});
}

}  // namespace tactile