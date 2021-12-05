#include "rename_layer_dialog.hpp"

#include <utility>  // move

#include <tactile_def.hpp>

#include "editor/events/layer_events.hpp"

namespace Tactile {

RenameLayerDialog::RenameLayerDialog() : ARenameDialog{"Rename Layer"} {}

void RenameLayerDialog::Show(const LayerID id, std::string oldName)
{
  mTargetId = id;
  mOldName = std::move(oldName);
  ARenameDialog::Show(*mOldName);
}

void RenameLayerDialog::OnAccept(entt::dispatcher& dispatcher, const std::string& input)
{
  dispatcher.enqueue<RenameLayerEvent>(mTargetId.value(), input);
}

auto RenameLayerDialog::IsInputValid(const entt::registry&, const std::string_view input)
    -> bool
{
  return !input.empty() && mOldName != input;
}

}  // namespace Tactile
