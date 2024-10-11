// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/tileset_dock.hpp"

#include <imgui.h>

#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/tileset_events.hpp"
#include "tactile/core/event/viewport_events.hpp"
#include "tactile/core/io/texture.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/tile/tileset.hpp"
#include "tactile/core/ui/canvas_overlay.hpp"
#include "tactile/core/ui/canvas_renderer.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/core/ui/common/overlays.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile::core::ui {
namespace {

void _push_tileset_overlay(const Registry& registry,
                           const EntityID tileset_id,
                           const CanvasRenderer& canvas_renderer)
{
  if (const OverlayScope overlay {"##TilesetOverlay", Float2 {1.0f, 0.0f}, 0.5f};
      overlay.is_open()) {
    const auto& viewport = registry.get<CViewport>(tileset_id);
    const auto& tileset = registry.get<CTileset>(tileset_id);
    const auto& tileset_instance = registry.get<CTilesetInstance>(tileset_id);

    ImGui::SeparatorText("Tileset");
    ImGui::Text("Tile range: [%d, %d)",
                tileset_instance.tile_range.first_id,
                tileset_instance.tile_range.first_id + tileset_instance.tile_range.count);
    ImGui::Text("Tile size: (%d, %d)", tileset.tile_size.x(), tileset.tile_size.y());
    ImGui::Text("Rows: %d", tileset.extent.rows);
    ImGui::Text("Columns: %d", tileset.extent.cols);

    push_viewport_info_section(viewport);
    push_viewport_mouse_info_section(canvas_renderer);
  }
}

void _push_tileset_tab(const Registry& registry,
                       const EntityID tileset_id,
                       EventDispatcher& dispatcher)
{
  const IdScope tileset_scope {tileset_id};

  const auto& meta = registry.get<CMeta>(tileset_id);
  const auto& viewport = registry.get<CViewport>(tileset_id);
  const auto& texture = registry.get<CTexture>(tileset_id);
  const auto& tileset = registry.get<CTileset>(tileset_id);
  const auto& tileset_instance = registry.get<CTilesetInstance>(tileset_id);

  if (const TabItemScope tab {meta.name.c_str()}; tab.is_open()) {
    const auto canvas_tl = to_float2(ImGui::GetCursorScreenPos());
    const auto canvas_br = canvas_tl + to_float2(ImGui::GetContentRegionAvail());
    const auto canvas_size = canvas_br - canvas_tl;

    if (canvas_size.x() == 0.0f || canvas_tl.y() == 0.0f) {
      return;
    }

    if (viewport.size != canvas_size) {
      dispatcher.push<UpdateViewportSizeEvent>(tileset_id, canvas_size);
    }

    const CanvasRenderer canvas_renderer {canvas_tl,
                                          canvas_br,
                                          tileset.extent,
                                          tileset.tile_size,
                                          viewport};

    constexpr UColor bg_color {50, 50, 50, 255};
    canvas_renderer.clear_canvas(bg_color);

    const auto texture_pos = canvas_renderer.to_screen_pos(Float2 {0, 0});
    const auto texture_size = vec_cast<Float2>(texture.size);

    auto* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddImage(texture.raw_handle,
                        to_imvec2(texture_pos),
                        to_imvec2(texture_pos + texture_size));

    canvas_renderer.draw_orthogonal_grid(kColorBlack);

    _push_tileset_overlay(registry, tileset_id, canvas_renderer);

    ImGui::InvisibleButton("##Canvas", to_imvec2(canvas_size), ImGuiMouseButton_Left);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f)) {
      const auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.0f);
      dispatcher.push<OffsetViewportEvent>(tileset_id, to_float2(-delta));
      ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
    }
  }
}

void _push_tileset_tabs(const Registry& registry,
                        const EntityID map_id,
                        EventDispatcher& dispatcher)
{
  if (const TabBarScope tabs {"##TilesetTabs"}; tabs.is_open()) {
    const auto& map = registry.get<CMap>(map_id);

    for (const auto tileset_id : map.attached_tilesets) {
      _push_tileset_tab(registry, tileset_id, dispatcher);
    }
  }
}

void _push_empty_view(const Language& language, EventDispatcher& dispatcher)
{
  prepare_for_vertically_centered_widgets(2);
  push_centered_label(language.get(StringID::kMapHasNoTilesets));
  if (push_horizontally_centered_button(language.get(StringID::kCreateTileset))) {
    dispatcher.push<ShowNewTilesetDialogEvent>();
  }
}

}  // namespace

void TilesetDock::push(const Language& language,
                       const MapDocument& document,
                       EventDispatcher& dispatcher)
{
  if (const Window dock {language.get(StringID::kTilesetDock)}; dock.is_open()) {
    const auto& registry = document.get_registry();
    const auto& document_info = registry.get<CDocumentInfo>();
    const auto& map = registry.get<CMap>(document_info.root);

    if (map.attached_tilesets.empty()) {
      _push_empty_view(language, dispatcher);
    }
    else {
      _push_tileset_tabs(registry, document_info.root, dispatcher);
    }
  }
}

}  // namespace tactile::core::ui
