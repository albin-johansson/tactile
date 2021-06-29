#include "tileset_content_widget.hpp"

#include "core/events/remove_tileset_event.hpp"
#include "core/events/select_tileset_event.hpp"
#include "core/map_document.hpp"
#include "gui/widgets/tilesets/tileset_viewport_widget.hpp"
#include "imgui.h"
#include "utils/scope_id.hpp"

namespace tactile {

void TilesetContentWidget(const MapDocument& document,
                          entt::dispatcher& dispatcher)
{
  const auto& tilesets = document.GetTilesets();
  if (ImGui::BeginTabBar("TilesetTabBar", ImGuiTabBarFlags_Reorderable))
  {
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

}  // namespace tactile
