#include "remove_column_shortcut.hpp"

#include "core/map.hpp"
#include "editor/events/map_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

RemoveColumnShortcut::RemoveColumnShortcut()
    : AShortcut{cen::scancodes::c, KMOD_LSHIFT | KMOD_LALT}
{}

void RemoveColumnShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveColumnEvent>();
}

auto RemoveColumnShortcut::IsEnabled(const Model& model) const -> bool
{
  if (const auto* registry = model.GetActiveRegistry()) {
    const auto& map = registry->ctx<Map>();
    return map.column_count > 1;
  }
  else {
    return false;
  }
}

}  // namespace Tactile
