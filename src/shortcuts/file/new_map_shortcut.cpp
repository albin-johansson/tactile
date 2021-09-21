#include "new_map_shortcut.hpp"

#include "gui/widgets/menus/file_menu.hpp"

namespace Tactile {

NewMapShortcut::NewMapShortcut() : AShortcut{cen::scancodes::n, cen::key_mod::left_ctrl}
{}

void NewMapShortcut::Activate(entt::dispatcher&)
{
  ShowAddMapDialog();
}

}  // namespace Tactile
