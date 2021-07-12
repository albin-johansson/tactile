#include "add_tileset_shortcut.hpp"

#include "core/model.hpp"
#include "gui/widgets/menus/edit_menu.hpp"

namespace Tactile {

AddTilesetShortcut::AddTilesetShortcut()
    : AShortcut{cen::scancodes::t, cen::key_mod::left_ctrl}
{}

void AddTilesetShortcut::Activate(entt::dispatcher& dispatcher)
{
  EnableTilesetDialog();
}

auto AddTilesetShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.GetActiveDocument() != nullptr;
}

}  // namespace Tactile
