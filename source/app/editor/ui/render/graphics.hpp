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

  void draw_translated_rect(const ImVec2& position,
                            const ImVec2& size,
                            uint32 color,
                            float thickness = 1.0f);

  void draw_translated_rect(const ImVec2& position,
                            const ImVec2& size,
                            const cen::color& color,
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

  void draw_translated_circle_with_shadow(const ImVec2& center,
                                          float radius,
                                          uint32 color,
                                          float thickness = 1.0f);

  void draw_translated_shadowed_ellipse(const Float2& center,
                                        const Float2& radius,
                                        const cen::color& color,
                                        float thickness = 1.0f);

  void draw_translated_ellipse_with_shadow(const ImVec2& center,
                                           const ImVec2& radius,
                                           uint32 color,
                                           float thickness = 1.0f);

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
  void render_translated_grid(const cen::color& color);

  void render_infinite_grid(const cen::color& color);

  void outline_contents(const cen::color& color);

  [[nodiscard]] auto from_matrix_to_absolute(int32 row, int32 column) const -> ImVec2;

  [[nodiscard]] auto is_intersecting_bounds(const ImVec2& position,
                                            const ImVec2& size) const -> bool;

  [[nodiscard]] auto is_within_translated_bounds(const ImVec2& position) const -> bool;

  [[nodiscard]] auto translate(const Float2& position) const -> Float2;
  [[nodiscard]] auto translate(const ImVec2& position) const -> ImVec2;

  [[nodiscard]] auto info() const -> const RenderInfo& { return mInfo; }

 private:
  RenderInfo mInfo;
  cen::frect mBoundsRect;
};

}  // namespace tactile::ui