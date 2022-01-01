#include "create_component_dialog.hpp"

#include <string>  // string

#include <fmt/format.h>

#include "editor/events/component_events.hpp"

namespace Tactile {

CreateComponentDialog::CreateComponentDialog() : AComponentNameDialog{"Create Component"}
{
  SetAcceptButtonLabel("Create");
}

void CreateComponentDialog::Open()
{
  Show(fmt::format("Component {}", mSuffix));
}

void CreateComponentDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CreateComponentDefEvent>(std::string{GetCurrentInput()});
  ++mSuffix;
}

}  // namespace Tactile