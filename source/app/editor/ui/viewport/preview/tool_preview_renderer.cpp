/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tool_preview_renderer.hpp"

#include "core/document/map_document.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/model.hpp"
#include "core/tool/ellipse_tool.hpp"
#include "core/tool/rectangle_tool.hpp"
#include "core/tool/stamp_tool.hpp"
#include "core/util/functional.hpp"
#include "editor/ui/render/graphics.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {

ToolPreviewRenderer::ToolPreviewRenderer(const DocumentModel& model,
                                         GraphicsCtx& graphics,
                                         const MouseInfo& mouse)
    : mModel {model}
    , mGraphics {graphics}
    , mMouseInfo {mouse}
{
}

void ToolPreviewRenderer::visit(const StampTool& tool)
{
  const auto& model = mModel.get();

  const auto& document = model.require_active_map();
  const auto& map = document.get_map();
  const auto& tilesets = map.get_tilesets();

  if (!map.is_active_layer(LayerType::TileLayer)) {
    return;
  }

  const auto tileset_id = tilesets.active_tileset_id().value();
  const auto& tileset_ref = tilesets.get_ref(tileset_id);

  if (!mMouseInfo.is_within_contents || !tileset_ref.get_selection()) {
    return;
  }

  // TODO implement preview when randomizer is enabled
  if (!tool.behaves_as_if_random(map)) {
    render_stamp_normal(map, tileset_ref);
  }
}

void ToolPreviewRenderer::render_stamp_normal(const Map& map,
                                              const TilesetRef& tileset_ref)
{
  auto& graphics = mGraphics.get();

  TACTILE_ASSERT(map.is_active_layer(LayerType::TileLayer));
  const auto layer_id = map.active_layer_id().value();
  const auto& layer = map.view_tile_layer(layer_id);

  const auto& selection = tileset_ref.get_selection().value();
  const auto selection_size = selection.end - selection.begin;
  const auto offset = selection_size / TilePos {2, 2};

  const auto& tileset = tileset_ref.view_tileset();
  const auto texture_id = tileset.texture_id();
  const auto& uv = tileset.uv_size();

  const auto origin = graphics.get_origin();
  const auto grid_size = graphics.get_grid_size();

  invoke_mn(selection_size.row(), selection_size.col(), [&, this](int32 row, int32 col) {
    const TilePos index {row, col};
    const auto preview_pos = mMouseInfo.position_in_viewport + index - offset;

    if (layer.is_valid(preview_pos)) {
      const auto real_pos = origin + preview_pos.as_vec2f() * grid_size;

      const auto uv_min = (selection.begin + index).as_vec2f() * uv;
      const auto uv_max = uv_min + uv;

      constexpr uint8 opacity = 150;
      graphics.render_image(texture_id, real_pos, grid_size, uv_min, uv_max, opacity);
    }
  });
}

void ToolPreviewRenderer::visit(const RectangleTool& tool)
{
  const auto& stroke = tool.get_stroke();
  if (stroke.has_value()) {
    auto& graphics = mGraphics.get();

    const auto pos = graphics.get_origin() + stroke->start;
    const auto size = stroke->current - stroke->start;

    graphics.draw_rect(pos + float2 {1, 1}, size, cen::colors::black, 1.0f);
    graphics.draw_rect(pos, size, cen::colors::yellow, 1.0f);
  }
}

void ToolPreviewRenderer::visit(const EllipseTool& tool)
{
  const auto& stroke = tool.get_stroke();
  if (stroke.has_value()) {
    auto& graphics = mGraphics.get();

    const auto radius = stroke->current - stroke->start;
    const auto center = stroke->start + radius;

    graphics.draw_ellipse(center + float2 {1, 1}, radius, cen::colors::black, 1.0f);
    graphics.draw_ellipse(center, radius, cen::colors::yellow, 1.0f);
  }
}

}  // namespace tactile::ui