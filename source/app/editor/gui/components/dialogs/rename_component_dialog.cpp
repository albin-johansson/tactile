#include "rename_component_dialog.hpp"

#include <utility>  // move

#include "editor/events/component_events.hpp"

namespace tactile {

RenameComponentDialog::RenameComponentDialog() : AComponentNameDialog{"Rename Component"}
{
  set_accept_button_label("Rename");
}

void RenameComponentDialog::Open(std::string previousName, const component_id id)
{
  mComponentId = id;
  Show(std::move(previousName));
}

void RenameComponentDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<rename_component_def_event>(mComponentId.value(),
                                                 std::string{GetCurrentInput()});
}

}  // namespace tactile