#include "rename_layer_dialog.hpp"

#include <utility>  // move

#include "editor/events/layer_events.hpp"

namespace tactile {

rename_layer_dialog::rename_layer_dialog() : string_input_dialog{"Rename Layer"}
{
  set_accept_button_label("Rename");
}

void rename_layer_dialog::show(const layer_id id, std::string oldName)
{
  mTargetId = id;
  mOldName = std::move(oldName);
  string_input_dialog::show(*mOldName);
}

void rename_layer_dialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<rename_layer_event>(mTargetId.value(), std::string{current_input()});
}

auto rename_layer_dialog::validate(const document_model&, std::string_view input) const
    -> bool
{
  return !input.empty() && mOldName != input;
}

}  // namespace tactile
