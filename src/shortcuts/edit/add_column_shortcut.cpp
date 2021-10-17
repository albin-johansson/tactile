#include "add_column_shortcut.hpp"

#include "core/model.hpp"
#include "gui/events/map_events.hpp"

namespace Tactile {

AddColumnShortcut::AddColumnShortcut()
    : AShortcut{cen::scancodes::c, cen::key_mod::left_alt}
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
