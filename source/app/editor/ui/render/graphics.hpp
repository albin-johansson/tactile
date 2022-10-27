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

#include <centurion/color.hpp>
#include <centurion/math.hpp>
#include <imgui.h>

#include "core/region.hpp"
#include "core/vocabulary.hpp"
#include "editor/ui/render/render_info.hpp"

namespace tactile::ui {

/// Provides a common simplified rendering API.
class Graphics final {
 public:
  explicit Graphics(const RenderInfo& info);

  void push_clip();

  void pop_clip();

  void clear(uint32 color = IM_COL32_BLACK);

  void draw_rect(const Float2& pos,
                 const Float2& size,
                 uint32 color,
                 float thickness = 1.0f);

  void draw_ellipse(const Float2& center,
                    const Float2& radius,
                    uint32 color,
                    float thickness = 1.0f);

  void draw_rect(const ImVec2& position,
                 const ImVec2& size,
                 uint32 color,
                 float thickness = 1.0f);

  void fill_rect(const ImVec2& position, const ImVec2& size, uint32 color);

  void draw_translated_rect(const ImVec2& position,
                            const ImVec2& size,
                            uint32 color,
                            float thickness = 1.0f);

  void fill_translated_rect(const ImVec2& position, const ImVec2& size, uint32 color);

  void draw_rect_with_shadow(const ImVec2& position,
                             const ImVec2& size,
                             uint32 color,
                             float thickness = 1.0f);

  void draw_translated_rect_with_shadow(const ImVec2& position,
                                        const ImVec2& size,
                                        uint32 color,
                                        float thickness = 1.0f);

  void draw_circle_with_shadow(const ImVec2& center,
                               float radius,
                               uint32 color,
                               float thickness = 1.0f);

  void draw_translated_circle_with_shadow(const ImVec2& center,
                                          float radius,
                                          uint32 color,
                                          float thickness = 1.0f);

  void draw_ellipse_with_shadow(const ImVec2& center,
                                const ImVec2& radius,
                                uint32 color,
                                float thickness = 1.0f);

  void draw_translated_ellipse_with_shadow(const ImVec2& center,
                                           const ImVec2& radius,
                                           uint32 color,
                                           float thickness = 1.0f);

  void render_image(uint texture,
                    const Float2& pos,
                    const Float2& size,
                    const Float2& uv_min,
                    const Float2& uv_max,
                    uint8 opacity);

  void render_image(uint texture, const ImVec2& position, const ImVec2& size);

  void render_image(uint texture,
                    const ImVec4& source,
                    const ImVec2& position,
                    const ImVec2& uv,
                    float opacity = 1.0f);

  void render_translated_image(uint texture,
                               const ImVec4& source,
                               const ImVec2& position,
                               const ImVec2& uv,
                               float opacity = 1.0f);

  void render_text(const char* text, const ImVec2& position, uint32 color);

  void render_translated_text(const char* text, const ImVec2& position, uint32 color);

  void render_centered_text(const char* text, const ImVec2& center, uint32 color);

  void render_translated_grid(uint32 color);

  [[nodiscard]] auto from_matrix_to_absolute(int32 row, int32 column) const -> ImVec2;

  [[nodiscard]] auto is_intersecting_bounds(const ImVec2& position,
                                            const ImVec2& size) const -> bool;

  [[nodiscard]] auto is_within_translated_bounds(const ImVec2& position) const -> bool;

  [[nodiscard]] auto translate(const ImVec2& position) const -> ImVec2;

  [[nodiscard]] auto origin() const -> const ImVec2& { return mOrigin; }

  [[nodiscard]] auto get_origin() const -> Float2 { return {mOrigin.x, mOrigin.y}; }

  [[nodiscard]] auto get_grid_size() const -> Float2
  {
    return {mViewportTileSize.x, mViewportTileSize.y};
  }

  [[nodiscard]] auto viewport_tile_size() const -> const ImVec2&
  {
    return mViewportTileSize;
  }

  [[nodiscard]] auto logical_tile_size() const -> const ImVec2&
  {
    return mLogicalTileSize;
  }

  [[nodiscard]] auto tile_size_ratio() const -> const ImVec2& { return mTileSizeRatio; }

  [[nodiscard]] auto bounds() const -> const Region& { return mBounds; }

  [[nodiscard]] auto info() const -> const RenderInfo& { return mInfo; }

 private:
  RenderInfo mInfo;
  ImVec2 mCanvasTL;
  ImVec2 mCanvasBR;
  ImVec2 mOrigin {0, 0};
  ImVec2 mViewportTileSize {32, 32};
  ImVec2 mLogicalTileSize {32, 32};
  ImVec2 mTileSizeRatio {1, 1};
  Region mBounds;
  cen::frect mBoundsRect;
};

}  // namespace tactile::ui