// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tileset_tabs.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/tile/tileset_bundle.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/document_events.hpp"
#include "model/event/property_events.hpp"
#include "model/event/tileset_events.hpp"
#include "model/model.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tileset_view.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

void update_context_menu(const DocumentModel& model,
                         const UUID& tileset_id,
                         entt::dispatcher& dispatcher)
{
  if (auto popup = Popup::for_item("##TilesetTabContext"); popup.is_open()) {
    const auto& lang = get_current_language();

    if (ImGui::MenuItem(lang.action.create_tileset.c_str())) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.inspect_tileset.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(tileset_id);
    }

    ImGui::Separator();

    if (Disable disable_if {model.is_open(tileset_id)};
        ImGui::MenuItem(lang.action.open_tileset.c_str())) {
      dispatcher.enqueue<OpenDocumentEvent>(tileset_id);
      dispatcher.enqueue<SelectDocumentEvent>(tileset_id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_tileset.c_str())) {
      dispatcher.enqueue<RemoveTilesetEvent>(tileset_id);
    }
  }
}

}  // namespace

void update_tileset_tabs(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(model.is_map_active());

  constexpr ImGuiTabBarFlags tab_bar_flags =
      ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

  if (const TabBar bar {"##TilesetTabBar", tab_bar_flags}; bar.is_open()) {
    if (ImGui::TabItemButton(TAC_ICON_ADD "##AddTilesetButton",
                             ImGuiTabItemFlags_Trailing)) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    const auto& map_document = model.require_active_map_document();
    const auto& map = map_document.get_map();
    const auto& tilesets = map.get_tileset_bundle();

    for (const auto& [tileset_id, ref]: tilesets) {
      const Scope scope {tileset_id};

      const auto& name = ref.get_tileset().get_ctx().name();
      const auto is_active = tilesets.get_active_tileset_id() == tileset_id;

      if (const TabItem item {name.c_str(),
                              nullptr,
                              is_active ? ImGuiTabItemFlags_SetSelected : 0};
          item.is_open()) {
        update_tileset_view(model, tileset_id, dispatcher);
      }

      if (ImGui::IsItemClicked(ImGuiMouseButton_Left) ||
          ImGui::IsItemClicked(ImGuiMouseButton_Right) || ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectTilesetEvent>(tileset_id);
      }
      else {
        update_context_menu(model, tileset_id, dispatcher);
      }
    }
  }
}

}  // namespace tactile::ui
