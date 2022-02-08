#include "rename_component_dialog.hpp"

#include <utility>  // move

#include "editor/events/component_events.hpp"

namespace tactile {

rename_component_dialog::rename_component_dialog()
    : component_name_dialog{"Rename Component"}
{
  set_accept_button_label("Rename");
}

void rename_component_dialog::show(std::string previousName, const component_id id)
{
  mComponentId = id;
  component_name_dialog::show(std::move(previousName));
}

void rename_component_dialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<rename_component_def_event>(mComponentId.value(),
                                                 std::string{current_input()});
}

}  // namespace tactile