#include "rename_property_dialog.hpp"

#include <utility>  // move

#include <tactile_def.hpp>

#include "core/systems/property_system.hpp"
#include "editor/events/property_events.hpp"
#include "editor/gui/dialogs/rename_dialog.hpp"

namespace Tactile {
namespace {

inline Maybe<std::string> gOldName;

}  // namespace

void UpdateRenamePropertyDialog(const entt::registry& registry,
                                entt::dispatcher& dispatcher)
{
  auto validator = [](const entt::registry& registry, const std::string_view name) {
    const auto& context = Sys::GetCurrentContext(registry);
    return !name.empty() && !Sys::HasPropertyWithName(registry, context, name);
  };

  auto callback = [](entt::dispatcher& dispatcher, std::string name) {
    dispatcher.enqueue<RenamePropertyEvent>(gOldName.value(), std::move(name));
  };

  UpdateRenameDialog("Rename property", registry, dispatcher, validator, callback);
}

void OpenRenamePropertyDialog(std::string name)
{
  gOldName = std::move(name);
  OpenRenameDialog("Rename property", *gOldName);
}

}  // namespace Tactile
