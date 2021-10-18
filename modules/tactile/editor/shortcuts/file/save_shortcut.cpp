#include "save_shortcut.hpp"

#include "tactile/editor/events/save_events.hpp"
#include "tactile/editor/model.hpp"

namespace Tactile {

SaveShortcut::SaveShortcut() : AShortcut{cen::scancodes::s, cen::key_mod::left_ctrl}
{}

void SaveShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SaveEvent>();
}

auto SaveShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanSaveDocument();
}

}  // namespace Tactile
