#include "rename_component_attribute_dialog.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

RenameComponentAttributeDialog::RenameComponentAttributeDialog()
    : AStringInputDialog{"Rename Component Attribute"}
{
  SetAcceptButtonLabel("Rename");
  SetInputHint("Attribute name");
}

void RenameComponentAttributeDialog::Open(std::string previousName, const ComponentID id)
{
  mComponentId = id;
  Show(std::move(previousName));
}

void RenameComponentAttributeDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RenameComponentAttributeEvent>(mComponentId.value(),
                                                    GetPreviousString(),
                                                    std::string{GetCurrentInput()});
}

auto RenameComponentAttributeDialog::Validate(const Model& model,
                                              const std::string_view input) const -> bool
{
  const auto& registry = model.GetActiveRegistryRef();
  return !input.empty() &&
         !Sys::IsComponentAttributeNameTaken(registry, mComponentId.value(), input);
}

}  // namespace Tactile