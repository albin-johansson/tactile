#include "rename_property_dialog.hpp"

#include <cassert>  // assert

#include "core/systems/property_system.hpp"
#include "editor/events/property_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

RenamePropertyDialog::RenamePropertyDialog() : ARenameDialog{"Rename Property"} {}

void RenamePropertyDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RenamePropertyEvent>(GetPreviousName(),
                                          std::string{GetCurrentInput()});
}

auto RenamePropertyDialog::Validate(const Model& model, std::string_view input) const
    -> bool
{
  const auto* registry = model.GetActiveRegistry();
  assert(registry);

  const auto& context = Sys::GetCurrentContext(*registry);
  return !input.empty() && !Sys::HasPropertyWithName(*registry, context, input);
}

}  // namespace Tactile
