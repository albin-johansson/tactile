#include "rename_layer_dialog.hpp"

#include <string>   // string
#include <utility>  // move

#include <tactile_def.hpp>

#include "editor/events/layer_events.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/dialogs/rename_dialog.hpp"

namespace Tactile {
namespace {

inline Maybe<LayerID> gTargetId;
inline Maybe<std::string> gOldName;

}  // namespace

void UpdateRenameLayerDialog(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  auto validator = [](const entt::registry&, const std::string_view name) {
    return !name.empty() && gOldName != name;
  };

  auto callback = [](entt::dispatcher& dispatcher, std::string name) {
    dispatcher.enqueue<RenameLayerEvent>(gTargetId.value(), std::move(name));
  };

  UpdateRenameDialog("Rename layer", registry, dispatcher, validator, callback);
}

void OpenRenameLayerDialog(const LayerID id, std::string oldName)
{
  gTargetId = id;
  gOldName = std::move(oldName);
  OpenRenameDialog("Rename layer", *gOldName);
}

}  // namespace Tactile
