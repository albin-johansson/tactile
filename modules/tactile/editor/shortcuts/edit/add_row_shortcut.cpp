#include "add_row_shortcut.hpp"

#include "editor/events/map_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

AddRowShortcut::AddRowShortcut() : AShortcut{cen::scancodes::r, KMOD_LALT}
{}

void AddRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddRowEvent>();
}

auto AddRowShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile
