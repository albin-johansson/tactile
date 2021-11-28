#include "open_map_shortcut.hpp"

#include "editor/gui/widgets/menus/file_menu.hpp"

namespace Tactile {

OpenMapShortcut::OpenMapShortcut() : AShortcut{cen::scancodes::o, KMOD_LCTRL}
{}

void OpenMapShortcut::Activate(entt::dispatcher&)
{
  ShowOpenMapDialog();
}

}  // namespace Tactile
