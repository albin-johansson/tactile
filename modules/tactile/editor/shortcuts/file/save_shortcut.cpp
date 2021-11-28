#include "save_shortcut.hpp"

#include "editor/events/save_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

SaveShortcut::SaveShortcut() : AShortcut{cen::scancodes::s, KMOD_LCTRL}
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
