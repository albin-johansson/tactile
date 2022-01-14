#include "rename_property_dialog.hpp"

#include "core/systems/property_system.hpp"
#include "editor/events/property_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

RenamePropertyDialog::RenamePropertyDialog() : AStringInputDialog{"Rename Property"}
{
  SetAcceptButtonLabel("Rename");
}

void RenamePropertyDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RenamePropertyEvent>(GetPreviousString(),
                                          std::string{GetCurrentInput()});
}

auto RenamePropertyDialog::Validate(const Model& model, std::string_view input) const
    -> bool
{
  const auto& registry = model.GetActiveRegistryRef();
  const auto& context = Sys::GetCurrentContext(registry);
  return !input.empty() && !Sys::HasPropertyWithName(registry, context, input);
}

}  // namespace Tactile
