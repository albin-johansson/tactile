#include "create_component_dialog.hpp"

#include <string>  // string

#include "editor/events/component_events.hpp"

namespace tactile {

CreateComponentDialog::CreateComponentDialog() : AComponentNameDialog{"Create Component"}
{
  SetAcceptButtonLabel("Create");
  SetInputHint("Component name");
}

void CreateComponentDialog::Open()
{
  Show("");
}

void CreateComponentDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CreateComponentDefEvent>(std::string{GetCurrentInput()});
}

}  // namespace tactile