#include "tileset_tabs.hpp"

#include <imgui.h>

#include "core/components/property_context.hpp"
#include "core/components/tileset.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/scoped.hpp"
#include "tileset_view.hpp"

namespace Tactile {
namespace {

constexpr auto gTabBarFlags =
    ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

void UpdateContextMenu(const TilesetID id, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginPopupContextItem("##TilesetTabContext")) {
    if (ImGui::MenuItem(TAC_ICON_ADD " Create New Tileset...")) {
      dispatcher.enqueue<ShowAddTilesetDialogEvent>();
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

void TilesetTabWidget::Update(const entt::registry& registry,
                              entt::dispatcher& dispatcher)
{
  if (ImGui::BeginTabBar("TilesetTabBar", gTabBarFlags)) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      dispatcher.enqueue<ShowAddTilesetDialogEvent>();
    }

    const auto& activeTileset = registry.ctx<ActiveTileset>();
    for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
      const Scoped::ID scope{tileset.id};

      const auto isActive = activeTileset.entity == entity;
      const auto& context = registry.get<PropertyContext>(entity);

      bool opened = true;
      if (ImGui::BeginTabItem(context.name.c_str(),
                              &opened,
                              isActive ? ImGuiTabItemFlags_SetSelected : 0))
      {
        mTilesetView.Update(registry, entity, dispatcher);
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

auto TilesetTabWidget::GetTilesetView() const -> const TilesetView&
{
  return mTilesetView;
}

}  // namespace Tactile
