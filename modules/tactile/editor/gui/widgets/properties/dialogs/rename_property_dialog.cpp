#include "rename_property_dialog.hpp"

#include <utility>  // move

#include <tactile-base/tactile_std.hpp>

#include "tactile/core/systems/property_system.hpp"
#include "tactile/editor/events/property_events.hpp"
#include "tactile/editor/gui/widgets/dialogs/rename_dialog.hpp"

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
