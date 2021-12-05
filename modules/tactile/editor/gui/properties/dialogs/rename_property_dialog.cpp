#include "rename_property_dialog.hpp"

#include "core/systems/property_system.hpp"
#include "editor/events/property_events.hpp"

namespace Tactile {

RenamePropertyDialog::RenamePropertyDialog() : ARenameDialog{"Rename Property"} {}

void RenamePropertyDialog::OnAccept(entt::dispatcher& dispatcher,
                                    const std::string& input)
{
  dispatcher.enqueue<RenamePropertyEvent>(GetPreviousName(), input);
}

auto RenamePropertyDialog::IsInputValid(const entt::registry& registry,
                                        std::string_view input) -> bool
{
  const auto& context = Sys::GetCurrentContext(registry);
  return !input.empty() && !Sys::HasPropertyWithName(registry, context, input);
}

}  // namespace Tactile
