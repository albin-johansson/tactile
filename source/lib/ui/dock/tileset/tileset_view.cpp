// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tileset_view.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/color.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/tile/tileset_ref.hpp"
#include "io/proto/settings.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/model.hpp"
#include "ui/conversions.hpp"
#include "ui/render/primitives.hpp"
#include "ui/render/renderer.hpp"
#include "ui/widget/rubber_band.hpp"

namespace tactile::ui {
namespace {

inline constexpr Color kRubberBandColor {0, 0x44, 0xCC, 100};

void update_viewport_offset(const TilesetRef& tileset_ref,
                            const Float2& viewport_size,
                            entt::dispatcher& dispatcher)
{
  const auto& tileset = tileset_ref.get_tileset();
  const auto texture_size = vector_cast<float>(tileset.texture().get_size());

  const Float2 min_offset = viewport_size - texture_size;
  const Float2 max_offset {};

  const auto& limits = tileset_ref.get_viewport().limits();
  if (!limits.has_value() || min_offset != limits->min_offset) {
    dispatcher.enqueue<UpdateTilesetViewportLimitsEvent>(tileset.get_uuid(),
                                                         min_offset,
                                                         max_offset);
  }

  ImGui::InvisibleButton("##TilesetViewInvisibleButton",
                         as_imvec2(viewport_size),
                         ImGuiButtonFlags_MouseButtonLeft |
                             ImGuiButtonFlags_MouseButtonMiddle |
                             ImGuiButtonFlags_MouseButtonRight);

  // This has no effect when users use touchpads, but that is handled separately
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    const Float2 delta {io.MouseDelta.x, io.MouseDelta.y};
    dispatcher.enqueue<OffsetTilesetViewportEvent>(tileset.get_uuid(), delta);
  }
}

void render_selection(const Region& selection, const ImVec2& min, const ImVec2& tile_size)
{
  const auto diff = selection.end - selection.begin;

  const auto origin = as_imvec2(selection.begin.as_vec2f()) * tile_size;
  const auto size = as_imvec2(diff.as_vec2f()) * tile_size;

  fill_rect(min + origin, size, kRubberBandColor);
}

}  // namespace

void update_tileset_view(const DocumentModel& model,
                         const UUID& tileset_id,
                         entt::dispatcher& dispatcher)
{
  const auto& document = model.require_active_map_document();
  const auto& map = document.get_map();

  const auto& tileset_ref = map.get_tileset_bundle().get_tileset_ref(tileset_id);
  const auto& tileset = tileset_ref.get_tileset();
  const auto& texture = tileset.texture();
  const auto& viewport = tileset_ref.get_viewport();

  const Renderer renderer {viewport, tileset};
  update_viewport_offset(tileset_ref, renderer.get_canvas_info().size, dispatcher);

  renderer.clear(get_settings().get_viewport_bg_color());

  const auto offset = as_imvec2(viewport.get_offset());
  const auto tile_size = as_imvec2(tileset.tile_size());

  if (const auto selection = ui_rubber_band(offset, tile_size)) {
    dispatcher.enqueue<SetTilesetSelectionEvent>(*selection);
  }

  renderer.push_clip();

  const auto position = ImGui::GetWindowDrawList()->GetClipRectMin() + offset;
  render_image(texture, position, as_imvec2(texture.get_size()));

  const auto& selection = tileset_ref.get_selection();
  if (selection.has_value()) {
    render_selection(*selection, position, tile_size);
  }

  renderer.render_translated_grid(get_settings().get_grid_color());
  renderer.pop_clip();
}

}  // namespace tactile::ui
