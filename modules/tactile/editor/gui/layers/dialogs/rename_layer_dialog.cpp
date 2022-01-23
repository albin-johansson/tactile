#include "rename_layer_dialog.hpp"

#include <utility>  // move

#include <tactile_def.hpp>

#include "editor/events/layer_events.hpp"

namespace tactile {

RenameLayerDialog::RenameLayerDialog() : AStringInputDialog{"Rename Layer"}
{
  SetAcceptButtonLabel("Rename");
}

void RenameLayerDialog::Show(const LayerID id, std::string oldName)
{
  mTargetId = id;
  mOldName = std::move(oldName);
  AStringInputDialog::Show(*mOldName);
}

void RenameLayerDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RenameLayerEvent>(mTargetId.value(), std::string{GetCurrentInput()});
}

auto RenameLayerDialog::Validate(const Model&, const std::string_view input) const -> bool
{
  return !input.empty() && mOldName != input;
}

}  // namespace tactile
