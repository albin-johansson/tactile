#include "rename_property_dialog.hpp"

#include <utility>  // move

#include <tactile_def.hpp>

#include "core/systems/property_system.hpp"
#include "editor/events/property_events.hpp"
#include "editor/gui/widgets/dialogs/rename_dialog.hpp"

namespace Tactile {
namespace {

inline Maybe<std::string> old_name;

}  // namespace

void UpdateRenamePropertyDialog(const entt::registry& registry,
                                entt::dispatcher& dispatcher)
{
  auto validator = [](const entt::registry& registry, const std::string_view name) {
    return !name.empty() && !Sys::HasPropertyWithName(registry, name);
  };

  auto callback = [](entt::dispatcher& dispatcher, std::string name) {
    dispatcher.enqueue<RenamePropertyEvent>(old_name.value(), std::move(name));
  };

  UpdateRenameDialog("Rename property", registry, dispatcher, validator, callback);
}

void OpenRenamePropertyDialog(std::string name)
{
  old_name = std::move(name);
  OpenRenameDialog("Rename property", *old_name);
}

}  // namespace Tactile
