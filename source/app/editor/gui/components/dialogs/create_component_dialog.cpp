#include "create_component_dialog.hpp"

#include <string>  // string

#include "editor/events/component_events.hpp"

namespace tactile {

create_component_dialog::create_component_dialog()
    : component_name_dialog{"Create Component"}
{
  set_accept_button_label("Create");
  set_input_hint("Component name");
}

void create_component_dialog::show()
{
  component_name_dialog::show("");
}

void create_component_dialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<create_component_def_event>(std::string{current_input()});
}

}  // namespace tactile