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

#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/util/functional.hpp"
#include "editor/ui/conversions.hpp"
#include "editor/ui/render/graphics.hpp"
#include "editor/ui/render/render.hpp"
#include "misc/assert.hpp"
#include "model/document/map_document.hpp"
#include "model/model.hpp"
#include "model/tool/ellipse_tool.hpp"
#include "model/tool/rectangle_tool.hpp"
#include "model/tool/stamp_tool.hpp"

namespace tactile::ui {
namespace {

inline constexpr uint8 stamp_preview_opacity = 150;

}  // namespace

ToolPreviewRenderer::ToolPreviewRenderer(const DocumentModel& model,
                                         Graphics& graphics,
                                         const MouseInfo& mouse)
    : mModel {model},
      mGraphics {graphics},
      mMouseInfo {mouse}
{
}

void ToolPreviewRenderer::visit(const StampTool& tool)
{
  const auto& model = mModel.get();

  const auto& document = model.require_active_map();
  const auto& map = document.get_map();
  const auto& tilesets = map.tileset_bundle();

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
  TACTILE_ASSERT(map.is_active_layer(LayerType::TileLayer));
  const auto layer_id = map.active_layer_id().value();
  const auto& layer = map.invisible_root().tile_layer(layer_id);

  const auto& selection = tileset_ref.get_selection().value();
  const auto selection_size = selection.end - selection.begin;
  const auto offset = selection_size / TilePos {2, 2};

  const auto& tileset = tileset_ref.view_tileset();

  auto& graphics = mGraphics.get();
  const auto origin = to_vec(graphics.info().origin);
  const auto grid_size = to_vec(graphics.info().grid_size);

  invoke_mn(selection_size.row(), selection_size.col(), [&, this](int32 row, int32 col) {
    const TilePos index {row, col};
    const auto preview_pos = mMouseInfo.position_in_viewport + index - offset;

    if (layer.is_valid(preview_pos)) {
      const auto rendered_position = origin + preview_pos.as_vec2f() * grid_size;
      graphics.render_tile(tileset,
                           selection.begin + index,
                           from_vec(rendered_position),
                           stamp_preview_opacity);
    }
  });
}

void ToolPreviewRenderer::visit(const RectangleTool& tool)
{
  const auto& stroke = tool.get_stroke();
  if (stroke.has_value()) {
    auto& graphics = mGraphics.get();

    const auto pos = graphics.info().origin + from_vec(stroke->start);
    const auto size = from_vec(stroke->current - stroke->start);

    draw_shadowed_rect(pos, size, to_u32(yellow));
  }
}

void ToolPreviewRenderer::visit(const EllipseTool& tool)
{
  const auto& stroke = tool.get_stroke();
  if (stroke.has_value()) {
    const auto radius = stroke->current - stroke->start;
    const auto center = stroke->start + radius;

    auto& graphics = mGraphics.get();
    graphics.draw_translated_shadowed_ellipse(from_vec(center),
                                              from_vec(radius),
                                              to_u32(yellow));
  }
}

}  // namespace tactile::ui