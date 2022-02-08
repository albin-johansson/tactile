#include "create_component_attribute_dialog.hpp"

#include <string>  // string

#include "core/systems/component_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/model.hpp"

namespace tactile {

CreateComponentAttributeDialog::CreateComponentAttributeDialog()
    : string_input_dialog{"Create Attribute"}
{
  set_accept_button_label("Create");
  set_input_hint("Attribute name");
}

void CreateComponentAttributeDialog::Open(const component_id id)
{
  mComponentId = id;
  show("");
}

auto CreateComponentAttributeDialog::validate(const Model& model,
                                              std::string_view input) const -> bool
{
  const auto& registry = model.get_active_registry();
  return !input.empty() &&
         !sys::is_component_attribute_name_taken(registry, mComponentId, input);
}

void CreateComponentAttributeDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<create_component_attr_event>(mComponentId,
                                                  std::string{current_input()});
}

}  // namespace tactile