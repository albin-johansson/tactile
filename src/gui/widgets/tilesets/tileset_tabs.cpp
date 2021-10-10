#include "tileset_tabs.hpp"

#include <imgui.h>

#include "core/components/property_context.hpp"
#include "core/components/tileset.hpp"
#include "events/tileset_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "tileset_view.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {
namespace {

constexpr auto tab_bar_flags =
    ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

void UpdateContextMenu(const TilesetID id, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginPopupContextItem("##TilesetTabContext")) {
    if (ImGui::MenuItem(TAC_ICON_ADD " Create New Tileset...")) {
      ShowTilesetDialog();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show Tileset Properties")) {
      dispatcher.enqueue<ShowTilesetPropertiesEvent>(id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Tileset")) {
      // TODO
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Tileset")) {
      dispatcher.enqueue<RemoveTilesetEvent>(id);
    }

    ImGui::EndPopup();
  }
}

}  // namespace

void UpdateTilesetTabs(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginTabBar("TilesetTabBar", tab_bar_flags)) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      ShowTilesetDialog();
    }

    const auto& activeTileset = registry.ctx<ActiveTileset>();
    for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
      const ScopeID uid{tileset.id};

      const auto isActive = activeTileset.entity == entity;
      const auto& context = registry.get<PropertyContext>(entity);

      bool opened = true;
      if (ImGui::BeginTabItem(context.name.c_str(),
                              &opened,
                              isActive ? ImGuiTabItemFlags_SetSelected : 0))
      {
        UpdateTilesetView(registry, entity, dispatcher);
        ImGui::EndTabItem();
      }

      if (!opened) {
        dispatcher.enqueue<RemoveTilesetEvent>(tileset.id);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectTilesetEvent>(tileset.id);
      }
      else {
        UpdateContextMenu(tileset.id, dispatcher);
      }
    }

    ImGui::EndTabBar();
  }
}

}  // namespace Tactile
