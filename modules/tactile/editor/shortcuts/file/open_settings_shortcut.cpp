#include "open_settings_shortcut.hpp"

#include "tactile/editor/gui/widgets/menus/edit_menu.hpp"

namespace Tactile {

OpenSettingsShortcut::OpenSettingsShortcut()
    : AShortcut{cen::scancodes::s, cen::key_mod::left_ctrl | cen::key_mod::left_alt}
{}

void OpenSettingsShortcut::Activate(entt::dispatcher&)
{
  ShowSettingsDialog();
}

}  // namespace Tactile
