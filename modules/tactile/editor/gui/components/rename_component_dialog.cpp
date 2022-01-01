#include "rename_component_dialog.hpp"

#include <utility>  // move

#include "editor/events/component_events.hpp"

namespace Tactile {

RenameComponentDialog::RenameComponentDialog() : AComponentNameDialog{"Rename Component"}
{
  SetAcceptButtonLabel("Rename");
}

void RenameComponentDialog::Open(std::string previousName, const ComponentID id)
{
  mComponentId = id;
  Show(std::move(previousName));
}

void RenameComponentDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RenameComponentDefEvent>(mComponentId.value(),
                                              std::string{GetCurrentInput()});
}

}  // namespace Tactile