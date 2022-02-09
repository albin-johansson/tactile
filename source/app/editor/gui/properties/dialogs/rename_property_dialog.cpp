#include "rename_property_dialog.hpp"

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"
#include "editor/events/property_events.hpp"
#include "editor/model.hpp"

namespace tactile {

RenamePropertyDialog::RenamePropertyDialog() : string_input_dialog{"Rename Property"}
{
  set_accept_button_label("Rename");
}

void RenamePropertyDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<rename_property_event>(previous_input(),
                                            std::string{current_input()});
}

auto RenamePropertyDialog::validate(const document_model& model,
                                    std::string_view input) const -> bool
{
  const auto& registry = model.get_active_registry();
  const auto& context = sys::current_context(registry);
  return !input.empty() && !sys::has_property_with_name(registry, context, input);
}

}  // namespace tactile
