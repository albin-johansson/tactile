#include "add_row_shortcut.hpp"

#include "tactile/editor/events/map_events.hpp"
#include "tactile/editor/model.hpp"

namespace Tactile {

AddRowShortcut::AddRowShortcut() : AShortcut{cen::scancodes::r, cen::key_mod::left_alt}
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
