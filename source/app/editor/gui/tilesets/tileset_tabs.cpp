#include "tileset_tabs.hpp"

#include <imgui.h>

#include "core/components/attribute_context.hpp"
#include "core/components/tileset.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/scoped.hpp"
#include "tileset_view.hpp"

namespace tactile {
namespace {

constexpr auto gTabBarFlags =
    ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

void UpdateContextMenu(const tileset_id id,
                       const entt::entity tilesetEntity,
                       entt::dispatcher& dispatcher)
{
  if (auto popup = scoped::Popup::ForItem("##TilesetTabContext"); popup.IsOpen()) {
    if (ImGui::MenuItem(TAC_ICON_ADD " Create New Tileset...")) {
      dispatcher.enqueue<show_add_tileset_dialog_event>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Tileset")) {
      dispatcher.enqueue<inspect_context_event>(tilesetEntity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Tileset")) {
      // TODO
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Tileset")) {
      dispatcher.enqueue<remove_tileset_event>(id);
    }
  }
}

}  // namespace

void TilesetTabWidget::Update(const entt::registry& registry,
                              entt::dispatcher& dispatcher)
{
  if (scoped::TabBar bar{"TilesetTabBar", gTabBarFlags}; bar.IsOpen()) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      dispatcher.enqueue<show_add_tileset_dialog_event>();
    }

    const auto& activeTileset = registry.ctx<comp::active_tileset>();
    for (auto&& [entity, tileset] : registry.view<comp::tileset>().each()) {
      const scoped::ID scope{tileset.id};

      const auto isActive = activeTileset.entity == entity;
      const auto& context = registry.get<comp::attribute_context>(entity);

      bool opened = true;
      if (scoped::TabItem item{context.name.c_str(),
                               &opened,
                               isActive ? ImGuiTabItemFlags_SetSelected : 0};
          item.IsOpen()) {
        mTilesetView.Update(registry, entity, dispatcher);
      }

      if (!opened) {
        dispatcher.enqueue<remove_tileset_event>(tileset.id);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<select_tileset_event>(tileset.id);
      }
      else {
        UpdateContextMenu(tileset.id, entity, dispatcher);
      }
    }
  }
}

auto TilesetTabWidget::GetTilesetView() const -> const TilesetView&
{
  return mTilesetView;
}

}  // namespace tactile
