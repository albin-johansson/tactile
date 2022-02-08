#include "rename_component_attribute_dialog.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/model.hpp"

namespace tactile {

RenameComponentAttributeDialog::RenameComponentAttributeDialog()
    : AStringInputDialog{"Rename Component Attribute"}
{
  SetAcceptButtonLabel("Rename");
  SetInputHint("Attribute name");
}

void RenameComponentAttributeDialog::Open(std::string previousName, const component_id id)
{
  mComponentId = id;
  Show(std::move(previousName));
}

void RenameComponentAttributeDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<rename_component_attr_event>(mComponentId.value(),
                                                  GetPreviousString(),
                                                  std::string{GetCurrentInput()});
}

auto RenameComponentAttributeDialog::Validate(const Model& model,
                                              const std::string_view input) const -> bool
{
  const auto& registry = model.get_active_registry();
  return !input.empty() &&
         !sys::is_component_attribute_name_taken(registry, mComponentId.value(), input);
}

}  // namespace tactile