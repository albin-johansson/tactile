#include "add_column_shortcut.hpp"

#include "editor/events/map_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

AddColumnShortcut::AddColumnShortcut() : AShortcut{cen::scancodes::c, KMOD_LALT}
{}

void AddColumnShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddColumnEvent>();
}

auto AddColumnShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile
