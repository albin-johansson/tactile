#include "remove_column_shortcut.hpp"

#include "core/ctx/map.hpp"
#include "core/model.hpp"
#include "gui/events/map_events.hpp"

namespace Tactile {

RemoveColumnShortcut::RemoveColumnShortcut()
    : AShortcut{cen::scancodes::c, cen::key_mod::left_shift | cen::key_mod::left_alt}
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
