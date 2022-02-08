#include "rename_component_attribute_dialog.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/model.hpp"

namespace tactile {

RenameComponentAttributeDialog::RenameComponentAttributeDialog()
    : string_input_dialog{"Rename Component Attribute"}
{
  set_accept_button_label("Rename");
  set_input_hint("Attribute name");
}

void RenameComponentAttributeDialog::Open(std::string previousName, const component_id id)
{
  mComponentId = id;
  show(std::move(previousName));
}

void RenameComponentAttributeDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<rename_component_attr_event>(mComponentId.value(),
                                                  previous_input(),
                                                  std::string{current_input()});
}

auto RenameComponentAttributeDialog::validate(const Model& model,
                                              std::string_view input) const -> bool
{
  const auto& registry = model.get_active_registry();
  return !input.empty() &&
         !sys::is_component_attribute_name_taken(registry, mComponentId.value(), input);
}

}  // namespace tactile