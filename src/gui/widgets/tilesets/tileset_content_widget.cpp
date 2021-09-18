#include "tileset_content_widget.hpp"

#include <imgui.h>

#include "core/components/property_context.hpp"
#include "core/components/tileset.hpp"
#include "events/tileset_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "tileset_viewport_widget.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {

void TilesetContentWidget(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

  if (ImGui::BeginTabBar("TilesetTabBar", flags)) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      ShowTilesetDialog();
    }

    const auto& currentTileset = registry.ctx<ActiveTileset>();
    for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
      const ScopeID uid{tileset.id};

      const auto isActive = currentTileset.entity == entity;
      const auto& context = registry.get<PropertyContext>(entity);

      bool opened = true;
      if (ImGui::BeginTabItem(context.name.c_str(),
                              &opened,
                              isActive ? ImGuiTabItemFlags_SetSelected : 0))
      {
        TilesetViewportWidget(registry, entity, dispatcher);
        ImGui::EndTabItem();
      }

      if (!opened) {
        dispatcher.enqueue<RemoveTilesetEvent>(tileset.id);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectTilesetEvent>(tileset.id);
      }
      else if (ImGui::BeginPopupContextItem("##TilesetTabContext")) {
        if (ImGui::MenuItem(TAC_ICON_ADD " Create new tileset...")) {
          ShowTilesetDialog();
        }

        ImGui::Separator();

        if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show tileset properties")) {
          dispatcher.enqueue<ShowTilesetPropertiesEvent>(tileset.id);
        }

        ImGui::Separator();

        if (ImGui::MenuItem(TAC_ICON_EDIT " Rename tileset")) {
          // TODO
        }

        ImGui::Separator();

        if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove tileset")) {
          dispatcher.enqueue<RemoveTilesetEvent>(tileset.id);
        }

        ImGui::EndPopup();
      }
    }

    ImGui::EndTabBar();
  }
}

}  // namespace Tactile
