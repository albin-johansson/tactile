#include "open_settings_shortcut.hpp"

#include "gui/widgets/menus/file_menu.hpp"

namespace Tactile {

OpenSettingsShortcut::OpenSettingsShortcut()
    : AShortcut{cen::scancodes::s, cen::key_mod::left_ctrl | cen::key_mod::left_alt}
{}

void OpenSettingsShortcut::Activate(entt::dispatcher& dispatcher)
{
  ShowSettingsDialog();
}

}  // namespace Tactile
