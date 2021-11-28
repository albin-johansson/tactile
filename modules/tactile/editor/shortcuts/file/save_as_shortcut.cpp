#include "save_as_shortcut.hpp"

#include "editor/events/save_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

SaveAsShortcut::SaveAsShortcut() : AShortcut{cen::scancodes::s, KMOD_LCTRL | KMOD_LSHIFT}
{}

void SaveAsShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<OpenSaveAsDialogEvent>();
}

auto SaveAsShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanSaveDocument();
}

}  // namespace Tactile
