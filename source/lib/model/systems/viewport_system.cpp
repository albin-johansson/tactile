/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "viewport_system.hpp"

#include <glm/common.hpp>

#include "components/viewport.hpp"
#include "model/systems/document_system.hpp"

namespace tactile::sys {
namespace {

inline constexpr Float2 kDefaultViewportTileSize = {64, 64};
inline constexpr float kMinViewportTileWidth = 4;

[[nodiscard]] auto _get_offset_delta(const Viewport& viewport) -> Float2
{
  const auto y_ratio = viewport.tile_size.y / viewport.tile_size.x;

  const auto dx = glm::round(glm::max(2.0f, viewport.tile_size.x * 0.05f));
  const auto dy = dx * y_ratio;

  return {dx, dy};
}

[[nodiscard]] auto _get_min_tile_size(const Viewport& viewport) -> Float2
{
  const auto y_ratio = viewport.tile_size.y / viewport.tile_size.x;
  return {kMinViewportTileWidth, kMinViewportTileWidth * y_ratio};
}

}  // namespace

void offset_viewport(Viewport& viewport, const Float2 delta)
{
  viewport.offset += delta;

  if (viewport.limits.has_value()) {
    viewport.offset = glm::clamp(viewport.offset,
                                 viewport.limits->min_offset,
                                 viewport.limits->max_offset);
  }
}

void reset_viewport_zoom(Viewport& viewport)
{
  viewport.tile_size = kDefaultViewportTileSize;
}

void increase_viewport_zoom(Viewport& viewport, const Float2 anchor_pos)
{
  const auto ratio = (anchor_pos - viewport.offset) / viewport.tile_size;

  viewport.tile_size += _get_offset_delta(viewport);
  viewport.offset = anchor_pos - (ratio * viewport.tile_size);
}

void decrease_viewport_zoom(Viewport& viewport, const Float2 anchor_pos)
{
  const auto ratio = (anchor_pos - viewport.offset) / viewport.tile_size;
  const auto min_tile_size = _get_min_tile_size(viewport);

  viewport.tile_size -= _get_offset_delta(viewport);
  viewport.tile_size = glm::max(viewport.tile_size, min_tile_size);

  viewport.offset = anchor_pos - (ratio * viewport.tile_size);
}

auto is_viewport_zoom_out_possible(const Model& model) -> bool
{
  const auto document_entity = sys::get_active_document(model);

  if (document_entity != kNullEntity) {
    const auto& viewport = model.get<Viewport>(document_entity);
    return viewport.can_zoom_out();
  }

  return false;
}

}  // namespace tactile::sys
