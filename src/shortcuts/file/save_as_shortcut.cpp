#include "save_as_shortcut.hpp"

#include "core/model.hpp"
#include "gui/events/save_events.hpp"

namespace Tactile {

SaveAsShortcut::SaveAsShortcut()
    : AShortcut{cen::scancodes::s, cen::key_mod::left_ctrl | cen::key_mod::left_shift}
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
