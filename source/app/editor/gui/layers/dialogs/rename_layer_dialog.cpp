#include "rename_layer_dialog.hpp"

#include <utility>  // move

#include "editor/events/layer_events.hpp"
#include "tactile.hpp"

namespace tactile {

RenameLayerDialog::RenameLayerDialog() : string_input_dialog{"Rename Layer"}
{
  set_accept_button_label("Rename");
}

void RenameLayerDialog::Show(const layer_id id, std::string oldName)
{
  mTargetId = id;
  mOldName = std::move(oldName);
  string_input_dialog::show(*mOldName);
}

void RenameLayerDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<rename_layer_event>(mTargetId.value(), std::string{current_input()});
}

auto RenameLayerDialog::validate(const document_model&, std::string_view input) const
    -> bool
{
  return !input.empty() && mOldName != input;
}

}  // namespace tactile
