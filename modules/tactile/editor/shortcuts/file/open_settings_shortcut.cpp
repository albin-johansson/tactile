#include "open_settings_shortcut.hpp"

#include "editor/gui/widgets/menus/edit_menu.hpp"

namespace Tactile {

OpenSettingsShortcut::OpenSettingsShortcut()
    : AShortcut{cen::scancodes::s, KMOD_LCTRL | KMOD_LALT}
{}

void OpenSettingsShortcut::Activate(entt::dispatcher&)
{
  ShowSettingsDialog();
}

}  // namespace Tactile
