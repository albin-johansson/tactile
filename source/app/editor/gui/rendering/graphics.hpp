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
#include "core/renderer.hpp"
#include "editor/fwd.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Provides a common simplified rendering API.
 */
class GraphicsCtx final : public IRenderer
{
 public:
  explicit GraphicsCtx(const RenderInfo& info);

  void draw_rect(const glm::vec2& pos,
                 const glm::vec2& size,
                 const cen::color& color,
                 float thickness) override;

  void draw_ellipse(const glm::vec2& center,
                    const glm::vec2& radius,
                    const cen::color& color,
                    float thickness) override;

  void render_image(uint texture,
                    const glm::vec2& pos,
                    const glm::vec2& size,
                    const glm::vec2& uvMin,
                    const glm::vec2& uvMax,
                    uint8 opacity) override;

  void push_clip();

  void pop_clip();

  void clear();

  void draw_rect(const ImVec2& position, const ImVec2& size);

  void fill_rect(const ImVec2& position, const ImVec2& size);

  void draw_translated_rect(const ImVec2& position, const ImVec2& size);

  void fill_translated_rect(const ImVec2& position, const ImVec2& size);

  void draw_rect_with_shadow(const ImVec2& position, const ImVec2& size);

  void draw_translated_rect_with_shadow(const ImVec2& position, const ImVec2& size);

  void draw_circle_with_shadow(const ImVec2& center, float radius);

  void draw_translated_circle_with_shadow(const ImVec2& center, float radius);

  void draw_ellipse_with_shadow(const ImVec2& center, const ImVec2& radius);

  void draw_translated_ellipse_with_shadow(const ImVec2& center, const ImVec2& radius);

  void render_image(uint texture, const ImVec2& position, const ImVec2& size);

  /**
   * \brief Renders a portion of a tileset texture.
   *
   * \param texture the identifier for the source texture that will be rendered.
   * \param source the region of the texture that will be rendered.
   * \param position the position of the rendered image on the screen.
   * \param uv the ratio between the tileset tile size and the texture size.
   */
  void render_image(uint texture,
                    const ImVec4& source,
                    const ImVec2& position,
                    const ImVec2& uv);

  void render_translated_image(uint texture,
                               const ImVec4& source,
                               const ImVec2& position,
                               const ImVec2& uv);

  void render_text(const char* text, const ImVec2& position);

  void render_translated_text(const char* text, const ImVec2& position);

  void render_centered_text(const char* text, const ImVec2& center);

  void render_translated_grid();

  void set_draw_color(const cen::color& color);

  void set_opacity(float opacity);

  void set_line_thickness(float thickness);

  [[nodiscard]] auto from_matrix_to_absolute(int32 row, int32 column) const -> ImVec2;

  [[nodiscard]] auto is_intersecting_bounds(const ImVec2& position,
                                            const ImVec2& size) const -> bool;

  [[nodiscard]] auto is_within_translated_bounds(const ImVec2& position) const -> bool;

  [[nodiscard]] auto translate(const ImVec2& position) const -> ImVec2;

  [[nodiscard]] auto origin() const -> const ImVec2& { return mOrigin; }

  [[nodiscard]] auto get_origin() const -> glm::vec2 override
  {
    return {mOrigin.x, mOrigin.y};
  }

  [[nodiscard]] auto get_grid_size() const -> glm::vec2 override
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

 private:
  ImVec2 mCanvasTL;
  ImVec2 mCanvasBR;
  ImVec2 mOrigin{0, 0};
  ImVec2 mViewportTileSize{32, 32};
  ImVec2 mLogicalTileSize{32, 32};
  ImVec2 mTileSizeRatio{1, 1};
  Region mBounds;
  cen::frect mBoundsRect;
  cen::color mDrawColor{cen::colors::black};
  float mLineThickness{1};
  uint8 mOpacity{0xFF};

  [[nodiscard]] auto get_draw_color() const -> uint32;

  [[nodiscard]] auto get_shadow_draw_color() const -> uint32;
};

}  // namespace tactile