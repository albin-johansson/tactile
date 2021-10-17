#include "add_tileset_shortcut.hpp"

#include "editor/gui/widgets/menus/map_menu.hpp"
#include "editor/model.hpp"

namespace Tactile {

AddTilesetShortcut::AddTilesetShortcut()
    : AShortcut{cen::scancodes::t, cen::key_mod::left_ctrl}
{}

void AddTilesetShortcut::Activate(entt::dispatcher&)
{
  ShowTilesetDialog();
}

auto AddTilesetShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile
