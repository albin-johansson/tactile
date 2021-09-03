#include "remove_row_shortcut.hpp"

#include "core/map.hpp"
#include "core/model.hpp"
#include "events/maps/remove_row_event.hpp"

namespace Tactile {

RemoveRowShortcut::RemoveRowShortcut()
    : AShortcut{cen::scancodes::r, cen::key_mod::left_alt | cen::key_mod::left_shift}
{}

void RemoveRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveRowEvent>();
}

auto RemoveRowShortcut::IsEnabled(const Model& model) const -> bool
{
  if (const auto* registry = model.GetActiveRegistry())
  {
    const auto& map = registry->ctx<Map>();
    return map.row_count > 1_row;
  }
  else
  {
    return false;
  }
}

}  // namespace Tactile
