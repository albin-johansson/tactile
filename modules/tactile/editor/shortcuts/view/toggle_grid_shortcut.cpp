#include "toggle_grid_shortcut.hpp"

#include "io/preferences.hpp"

namespace Tactile {

ToggleGridShortcut::ToggleGridShortcut() : AShortcut{cen::scancodes::g, KMOD_LCTRL}
{}

void ToggleGridShortcut::Activate(entt::dispatcher&)
{
  Prefs::SetShowGrid(!Prefs::GetShowGrid());
}

}  // namespace Tactile
