#include "create_component_dialog.hpp"

#include <string>  // string

#include "editor/events/component_events.hpp"

namespace tactile {

CreateComponentDialog::CreateComponentDialog() : AComponentNameDialog{"Create Component"}
{
  set_accept_button_label("Create");
  SetInputHint("Component name");
}

void CreateComponentDialog::Open()
{
  Show("");
}

void CreateComponentDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<create_component_def_event>(std::string{GetCurrentInput()});
}

}  // namespace tactile