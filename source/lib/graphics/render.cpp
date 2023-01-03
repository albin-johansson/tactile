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

#include "render.hpp"

#include <cmath>    // sin, cos
#include <numbers>  // pi_v

#include "common/debug/assert.hpp"
#include "core/texture.hpp"
#include "ui/textures.hpp"

namespace tactile::ui {
namespace {

inline constexpr int ellipse_segments = 50;

void path_elliptical_arc_to(ImDrawList* self,
                            const ImVec2& center,
                            const ImVec2& radius,
                            const float arc_min,
                            const float arc_max,
                            const int n_segments)
{
  for (auto i = 0; i <= n_segments; ++i) {
    const auto arc_diff = arc_max - arc_min;
    const auto n_segments_f = static_cast<float>(n_segments);
    const auto a = arc_min + (static_cast<float>(i) / n_segments_f) * arc_diff;

    const ImVec2 pos {center.x + std::cos(a) * radius.x,
                      center.y + std::sin(a) * radius.y};
    self->PathLineTo(pos);
  }
}

void add_ellipse(ImDrawList* self,
                 const ImVec2& center,
                 const ImVec2& radius,
                 const uint32 color,
                 const int n_segments,
                 const float thickness = 1.0f)
{
  if ((color & IM_COL32_A_MASK) == 0 || n_segments <= 2) {
    return;
  }

  constexpr auto tau = std::numbers::pi_v<float> * 2.0f;

  const auto n_segments_f = static_cast<float>(n_segments);

  // Because we are filling a closed shape we remove 1 from the count of segments/points
  const float arc_max = tau * (n_segments_f - 1.0f) / n_segments_f;
  path_elliptical_arc_to(self, center, radius, 0.0f, arc_max, n_segments - 1);

  self->PathStroke(color, true, thickness);
}

}  // namespace

void draw_rect(const ImVec2& position,
               const ImVec2& size,
               const uint32 color,
               const float thickness)
{
  ImGui::GetWindowDrawList()->AddRect(position, position + size, color, 0, 0, thickness);
}

void fill_rect(const ImVec2& position, const ImVec2& size, const uint32 color)
{
  ImGui::GetWindowDrawList()->AddRectFilled(position, position + size, color);
}

void draw_circle(const ImVec2& center,
                 const float radius,
                 const uint32 color,
                 const float thickness)
{
  ImGui::GetWindowDrawList()->AddCircle(center, radius, color, 0, thickness);
}

void draw_ellipse(const ImVec2& center,
                  const ImVec2& radius,
                  const uint32 color,
                  const float thickness)
{
  add_ellipse(ImGui::GetWindowDrawList(),
              center,
              radius,
              color,
              ellipse_segments,
              thickness);
}

void draw_shadowed_ellipse(const ImVec2& center,
                           const ImVec2& radius,
                           const uint32 color,
                           const float thickness)
{
  if (radius.x == radius.y) {
    draw_shadowed_circle(center, radius.x, color, thickness);
  }
  else {
    draw_ellipse(center + ImVec2 {0, thickness}, radius, IM_COL32_BLACK, thickness);
    draw_ellipse(center, radius, color, thickness);
  }
}

void render_image(const Texture& texture,
                  const ImVec2& position,
                  const ImVec2& size,
                  const ImVec2& uv_min,
                  const ImVec2& uv_max,
                  const uint8 opacity)
{
  ImGui::GetWindowDrawList()->AddImage(to_texture_id(texture.get_id()),
                                       position,
                                       position + size,
                                       uv_min,
                                       uv_max,
                                       IM_COL32(0xFF, 0xFF, 0xFF, opacity));
}

void render_text(const char* text, const ImVec2& position, const uint32 color)
{
  TACTILE_ASSERT(text);
  ImGui::GetWindowDrawList()->AddText(position, color, text);
}

}  // namespace tactile::ui
