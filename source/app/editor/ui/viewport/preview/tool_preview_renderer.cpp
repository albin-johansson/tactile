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
#include "editor/ui/rendering/graphics.hpp"

namespace tactile::ui {

ToolPreviewRenderer::ToolPreviewRenderer(const DocumentModel& model,
                                         GraphicsCtx&         graphics,
                                         const MouseInfo&     mouse)
    : mModel {model}
    , mGraphics {graphics}
    , mMouseInfo {mouse}
{}

void ToolPreviewRenderer::visit(const StampTool& tool)
{
  const auto& model = mModel.get();

  const auto& document = model.require_active_map();
  const auto& map = document.get_map();
  const auto& tilesets = map.get_tilesets();

  const auto  tilesetId = tilesets.active_tileset_id().value();
  const auto& tilesetRef = tilesets.get_ref(tilesetId);

  if (!mMouseInfo.is_within_contents || !tilesetRef.get_selection()) {
    return;
  }

  // TODO implement preview when randomizer is enabled
  if (!tool.behaves_as_if_random(map)) {
    render_stamp_normal(map, tilesetRef);
  }
}

void ToolPreviewRenderer::render_stamp_normal(const Map&        map,
                                              const TilesetRef& tilesetRef)
{
  auto& graphics = mGraphics.get();

  const auto  layerId = map.active_layer_id().value();
  const auto& layer = map.view_tile_layer(layerId);

  const auto& selection = tilesetRef.get_selection().value();
  const auto  selectionSize = selection.end - selection.begin;
  const auto  offset = selectionSize / TilePos {2, 2};

  const auto& tileset = tilesetRef.view_tileset();
  const auto  textureId = tileset.texture_id();
  const auto& uv = tileset.uv_size();

  const auto origin = graphics.get_origin();
  const auto gridSize = graphics.get_grid_size();

  invoke_mn(selectionSize.row(), selectionSize.col(), [&, this](int32 row, int32 col) {
    const TilePos index {row, col};
    const auto    previewPos = mMouseInfo.position_in_viewport + index - offset;

    if (layer.is_valid(previewPos)) {
      const auto realPos = origin + previewPos.as_vec2f() * gridSize;

      const auto uvMin = (selection.begin + index).as_vec2f() * uv;
      const auto uvMax = uvMin + uv;

      constexpr uint8 opacity = 150;
      graphics.render_image(textureId, realPos, gridSize, uvMin, uvMax, opacity);
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

    graphics.draw_rect(pos + Vector2f {1, 1}, size, cen::colors::black, 1.0f);
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

    graphics.draw_ellipse(center + Vector2f {1, 1}, radius, cen::colors::black, 1.0f);
    graphics.draw_ellipse(center, radius, cen::colors::yellow, 1.0f);
  }
}

}  // namespace tactile::ui