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

/// This header provides low-level rendering functions.

#pragma once

#include <centurion/fwd.hpp>
#include <imgui.h>

#include "core/type/math.hpp"
#include "core/vocabulary.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Texture)

namespace tactile::ui {

void draw_rect(const ImVec2& position,
               const ImVec2& size,
               uint32 color,
               float thickness = 1.0f);

void draw_rect(const Float2& position,
               const Float2& size,
               const cen::color& color,
               float thickness = 1.0f);

void fill_rect(const ImVec2& position, const ImVec2& size, uint32 color);

void fill_rect(const Float2& position, const Float2& size, const cen::color& color);

void draw_circle(const ImVec2& center,
                 float radius,
                 uint32 color,
                 float thickness = 1.0f);

void draw_circle(const Float2& center,
                 float radius,
                 const cen::color& color,
                 float thickness = 1.0f);

void draw_shadowed_circle(const ImVec2& center,
                          float radius,
                          uint32 color,
                          float thickness = 1.0f);

void draw_shadowed_circle(const Float2& center,
                          float radius,
                          const cen::color& color,
                          float thickness = 1.0f);

void draw_ellipse(const ImVec2& center,
                  const ImVec2& radius,
                  uint32 color,
                  float thickness = 1.0f);

void draw_ellipse(const Float2& center,
                  const Float2& radius,
                  const cen::color& color,
                  float thickness = 1.0f);

void draw_shadowed_ellipse(const ImVec2& center,
                           const ImVec2& radius,
                           uint32 color,
                           float thickness = 1.0f);

void draw_shadowed_ellipse(const Float2& center,
                           const Float2& radius,
                           const cen::color& color,
                           float thickness = 1.0f);

/// Renders a region of a texture.
///
/// \details
/// To render the full texture, you could use uv_min = (0, 0) and uv_max = (1, 1). That
/// is, just omit the UV parameters.
///
/// \param texture the source texture.
/// \param position the position of the rendered texture.
/// \param size the size of the rendered texture.
/// \param uv_min the normalized top-left corner of the texture region to render.
/// \param uv_max the normalized bottom-right corner of the texture region to render.
/// \param opacity the opacity of the rendered texture, in the interval [0, 255].
void render_image(const Texture& texture,
                  const Float2& position,
                  const Float2& size,
                  const Float2& uv_min = {0, 0},
                  const Float2& uv_max = {1, 1},
                  uint8 opacity = 255);

}  // namespace tactile::ui
