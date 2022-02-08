#include "rename_property_dialog.hpp"

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"
#include "editor/events/property_events.hpp"
#include "editor/model.hpp"

namespace tactile {

RenamePropertyDialog::RenamePropertyDialog() : AStringInputDialog{"Rename Property"}
{
  SetAcceptButtonLabel("Rename");
}

void RenamePropertyDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<rename_property_event>(GetPreviousString(),
                                            std::string{GetCurrentInput()});
}

auto RenamePropertyDialog::Validate(const Model& model, std::string_view input) const
    -> bool
{
  const auto& registry = model.get_active_registry();
  const auto& context = sys::current_context(registry);
  return !input.empty() && !sys::has_property_with_name(registry, context, input);
}

}  // namespace tactile
