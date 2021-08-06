#include "tileset_content_widget.hpp"

#include <imgui.h>

#include "core/map_document.hpp"
#include "events/tilesets/remove_tileset_event.hpp"
#include "events/tilesets/select_tileset_event.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "gui/widgets/tilesets/tileset_viewport_widget.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {

void TilesetContentWidget(const MapDocument& document, entt::dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

  const auto& tilesets = document.GetTilesets();
  if (ImGui::BeginTabBar("TilesetTabBar", flags))
  {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing))
    {
      EnableTilesetDialog();
    }

    for (const auto& [id, tileset] : tilesets)
    {
      const ScopeID uid{id};

      bool opened = true;
      const auto isActive = tilesets.GetActiveTilesetId() == id;
      if (ImGui::BeginTabItem(tileset->GetName().data(),
                              &opened,
                              isActive ? ImGuiTabItemFlags_SetSelected : 0))
      {
        TilesetViewportWidget(*tileset, dispatcher);
        ImGui::EndTabItem();
      }

      if (!opened)
      {
        dispatcher.enqueue<RemoveTilesetEvent>(id);
      }
      else if (ImGui::IsItemActivated())
      {
        dispatcher.enqueue<SelectTilesetEvent>(id);
      }
    }

    ImGui::EndTabBar();
  }
}

}  // namespace Tactile
