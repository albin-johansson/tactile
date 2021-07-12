#include "toggle_grid_shortcut.hpp"

#include "io/preferences.hpp"

namespace Tactile {

ToggleGridShortcut::ToggleGridShortcut()
    : AShortcut{cen::scancodes::g, cen::key_mod::left_ctrl}
{}

void ToggleGridShortcut::Activate(entt::dispatcher& dispatcher)
{
  Prefs::SetShowGrid(!Prefs::GetShowGrid());
}

}  // namespace Tactile
