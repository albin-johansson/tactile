#include "tileset_dock.hpp"

#include <imgui.h>

#include "core/components/tileset.hpp"
#include "events/tilesets/remove_tileset_event.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/centered_button.hpp"
#include "gui/widgets/common/centered_text.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "io/preferences.hpp"
#include "tileset_content_widget.hpp"

namespace Tactile {

void UpdateTilesetDock(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowTilesetDock())
  {
    return;
  }

  bool isVisible = Prefs::GetShowTilesetDock();
  if (ImGui::Begin("Tilesets",
                   &isVisible,
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
  {
    const auto view = registry.view<Tileset>();
    if (view.size() != 0)
    {
      TilesetContentWidget(registry, dispatcher);
    }
    else
    {
      PrepareVerticalAlignmentCenter(2);

      CenteredText("No available tilesets!");
      if (CenteredButton(TAC_ICON_TILESET " Create tileset..."))
      {
        ShowTilesetDialog();
      }
    }
  }

  Prefs::SetShowTilesetDock(isVisible);
  ImGui::End();
}

}  // namespace Tactile
