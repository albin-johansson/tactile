#include "add_tileset_shortcut.hpp"

#include "tactile/editor/gui/widgets/menus/map_menu.hpp"
#include "tactile/editor/model.hpp"

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
