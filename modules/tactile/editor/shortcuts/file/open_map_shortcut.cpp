#include "open_map_shortcut.hpp"

#include "tactile/editor/gui/widgets/menus/file_menu.hpp"

namespace Tactile {

OpenMapShortcut::OpenMapShortcut() : AShortcut{cen::scancodes::o, cen::key_mod::left_ctrl}
{}

void OpenMapShortcut::Activate(entt::dispatcher&)
{
  ShowOpenMapDialog();
}

}  // namespace Tactile
