#include "rename_layer_dialog.hpp"

#include <string>   // string
#include <utility>  // move

#include "common/maybe.hpp"
#include "events/layer_events.hpp"
#include "gui/widgets/common/button.hpp"
#include "gui/widgets/dialogs/rename_dialog.hpp"

namespace Tactile {
namespace {

inline Maybe<LayerID> target_id;
inline Maybe<std::string> old_name;

}  // namespace

void UpdateRenameLayerDialog(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  auto validator = [](const entt::registry&, const std::string_view name) {
    return !name.empty() && old_name != name;
  };

  auto callback = [](entt::dispatcher& dispatcher, std::string name) {
    dispatcher.enqueue<RenameLayerEvent>(target_id.value(), std::move(name));
  };

  UpdateRenameDialog("Rename layer", registry, dispatcher, validator, callback);
}

void OpenRenameLayerDialog(const LayerID id, std::string oldName)
{
  target_id = id;
  old_name = std::move(oldName);
  OpenRenameDialog("Rename layer", *old_name);
}

}  // namespace Tactile
