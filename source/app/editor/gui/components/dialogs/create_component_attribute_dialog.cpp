#include "create_component_attribute_dialog.hpp"

#include <string>  // string

#include "core/systems/component_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/model.hpp"

namespace tactile {

create_component_attribute_dialog::create_component_attribute_dialog()
    : string_input_dialog{"Create Attribute"}
{
  set_accept_button_label("Create");
  set_input_hint("Attribute name");
}

void create_component_attribute_dialog::show(const component_id id)
{
  mComponentId = id;
  string_input_dialog::show("");
}

auto create_component_attribute_dialog::validate(const document_model& model,
                                                 std::string_view input) const -> bool
{
  const auto& registry = model.get_active_registry();
  return !input.empty() &&
         !sys::is_component_attribute_name_taken(registry, mComponentId, input);
}

void create_component_attribute_dialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<create_component_attr_event>(mComponentId,
                                                  std::string{current_input()});
}

}  // namespace tactile