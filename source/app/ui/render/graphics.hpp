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

#pragma once

#include <centurion/math.hpp>
#include <imgui.h>

#include "core/region.hpp"
#include "core/vocabulary.hpp"
#include "render.hpp"
#include "render_info.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Tileset)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, Texture)

namespace tactile::ui {

/// Represents a graphics context associated with a canvas.
class Graphics final {
 public:
  explicit Graphics(const RenderInfo& info);

  /// Enables clipping for the visible canvas area.
  void push_canvas_clip() const;

  /// Disables clipping.
  void pop_clip() const;

  void clear(uint32 color = IM_COL32_BLACK) const;

  /// Renders an outline of the content region.
  void outline_contents(uint32 color) const;

  void render_translated_grid(uint32 color);

  /// Renders a seemingly infinite grid, aligned with the content tiles.
  /// \param color the color of the rendered grid.
  void render_infinite_grid(uint32 color);

  /// Renders a single tile from a tileset.
  ///
  /// \param tileset the source tileset.
  /// \param tile_position the position of the tile in the tileset.
  /// \param position the rendered position of the tile.
  /// \param opacity the opacity of the tile, in the interval [0, 255].
  void render_tile(const Tileset& tileset,
                   const TilePos& tile_position,
                   const ImVec2& position,
                   uint8 opacity = 255) const;

  void draw_translated_rect(const ImVec2& position,
                            const ImVec2& size,
                            const uint32 color,
                            const float thickness = 1.0f) const
  {
    draw_rect(translate(position), size, color, thickness);
  }

  void draw_translated_shadowed_rect(const ImVec2& position,
                                     const ImVec2& size,
                                     const uint32 color,
                                     const float thickness = 1.0f) const
  {
    draw_shadowed_rect(translate(position), size, color, thickness);
  }

  void draw_translated_shadowed_circle(const ImVec2& center,
                                       const float radius,
                                       const uint32 color,
                                       const float thickness = 1.0f) const
  {
    draw_shadowed_circle(translate(center), radius, color, thickness);
  }

  void draw_translated_shadowed_ellipse(const ImVec2& center,
                                        const ImVec2& radius,
                                        const uint32 color,
                                        const float thickness = 1.0f) const
  {
    draw_shadowed_ellipse(translate(center), radius, color, thickness);
  }

  void render_translated_text(const char* text,
                              const ImVec2& position,
                              const uint32 color) const
  {
    render_text(text, translate(position), color);
  }

  /// Translates a tile position to an absolute position on the canvas.
  [[nodiscard]] auto from_matrix_to_absolute(TilePos pos) const -> ImVec2;

  [[nodiscard]] auto is_intersecting_bounds(ImVec2 position, ImVec2 size) const -> bool;

  [[nodiscard]] auto is_within_translated_bounds(ImVec2 position) const -> bool;

  [[nodiscard]] auto translate(const ImVec2 position) const -> ImVec2
  {
    return mInfo.origin + position;
  }

  [[nodiscard]] auto info() const noexcept -> const RenderInfo& { return mInfo; }

 private:
  RenderInfo mInfo;
  cen::frect mBoundsRect;
};

}  // namespace tactile::ui