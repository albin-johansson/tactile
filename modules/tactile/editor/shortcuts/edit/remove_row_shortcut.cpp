#include "remove_row_shortcut.hpp"

#include "core/map.hpp"
#include "editor/events/map_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

RemoveRowShortcut::RemoveRowShortcut()
    : AShortcut{cen::scancodes::r, KMOD_LALT | KMOD_LSHIFT}
{}

void RemoveRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveRowEvent>();
}

auto RemoveRowShortcut::IsEnabled(const Model& model) const -> bool
{
  if (const auto* registry = model.GetActiveRegistry()) {
    const auto& map = registry->ctx<Map>();
    return map.row_count > 1;
  }
  else {
    return false;
  }
}

}  // namespace Tactile
