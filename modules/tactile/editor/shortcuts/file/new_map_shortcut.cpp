#include "new_map_shortcut.hpp"

#include "editor/gui/widgets/menus/file_menu.hpp"

namespace Tactile {

NewMapShortcut::NewMapShortcut() : AShortcut{cen::scancodes::n, KMOD_LCTRL}
{}

void NewMapShortcut::Activate(entt::dispatcher&)
{
  ShowAddMapDialog();
}

}  // namespace Tactile
