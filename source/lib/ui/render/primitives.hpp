// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

/// This header provides low-level rendering functions.

#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include "core/texture.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/meta/color.hpp"
#include "ui/conversions.hpp"

namespace tactile {

inline void render_text(const char* text, const ImVec2& position, const Color& color)
{
  TACTILE_ASSERT(text);
  ImGui::GetWindowDrawList()->AddText(position, to_u32(color), text);
}

inline void render_text(const char* text, const Float2& position, const Color& color)
{
  render_text(text, as_imvec2(position), color);
}

inline void render_shadowed_text(const char* text,
                                 const ImVec2& position,
                                 const Color& color,
                                 const float shadow_offset = 1.0f)
{
  render_text(text, position + ImVec2 {shadow_offset, shadow_offset}, kColorBlack);
  render_text(text, position, color);
}

inline void render_shadowed_text(const char* text,
                                 const Float2& position,
                                 const Color& color,
                                 const float shadow_offset = 1.0f)
{
  render_shadowed_text(text, as_imvec2(position), color, shadow_offset);
}

inline void draw_rect(const ImVec2& position,
                      const ImVec2& size,
                      const Color& color,
                      const float thickness = 1.0f)
{
  auto* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddRect(position, position + size, to_u32(color), 0, 0, thickness);
}

inline void draw_rect(const Float2& position,
                      const Float2& size,
                      const Color& color,
                      const float thickness = 1.0f)
{
  draw_rect(as_imvec2(position), as_imvec2(size), color, thickness);
}

inline void draw_shadowed_rect(const ImVec2& position,
                               const ImVec2& size,
                               const Color& color,
                               const float thickness = 1.0f)
{
  draw_rect(position + ImVec2 {thickness, thickness}, size, kColorBlack, thickness);
  draw_rect(position, size, color, thickness);
}

inline void draw_shadowed_rect(const Float2& position,
                               const Float2& size,
                               const Color& color,
                               const float thickness = 1.0f)
{
  draw_rect(position + Float2 {thickness, thickness}, size, kColorBlack, thickness);
  draw_rect(position, size, color, thickness);
}

inline void fill_rect(const ImVec2& position, const ImVec2& size, const Color& color)
{
  ImGui::GetWindowDrawList()->AddRectFilled(position, position + size, to_u32(color));
}

inline void fill_rect(const Float2& position, const Float2& size, const Color& color)
{
  fill_rect(as_imvec2(position), as_imvec2(size), color);
}

inline void draw_circle(const ImVec2& center,
                        const float radius,
                        const Color& color,
                        const float thickness = 1.0f)
{
  ImGui::GetWindowDrawList()->AddCircle(center, radius, to_u32(color), 0, thickness);
}

inline void draw_circle(const Float2& center,
                        const float radius,
                        const Color& color,
                        const float thickness = 1.0f)
{
  draw_circle(as_imvec2(center), radius, color, thickness);
}

inline void draw_shadowed_circle(const ImVec2& center,
                                 const float radius,
                                 const Color& color,
                                 const float thickness = 1.0f)
{
  draw_circle(center + ImVec2 {0, thickness}, radius, kColorBlack, thickness);
  draw_circle(center, radius, color, thickness);
}

inline void draw_shadowed_circle(const Float2& center,
                                 const float radius,
                                 const Color& color,
                                 const float thickness = 1.0f)
{
  draw_circle(center + Float2 {0, thickness}, radius, kColorBlack, thickness);
  draw_circle(center, radius, color, thickness);
}

void draw_ellipse(const ImVec2& center,
                  const ImVec2& radius,
                  const Color& color,
                  float thickness = 1.0f);

inline void draw_ellipse(const Float2& center,
                         const Float2& radius,
                         const Color& color,
                         const float thickness = 1.0f)
{
  draw_ellipse(as_imvec2(center), as_imvec2(radius), color, thickness);
}

inline void draw_shadowed_ellipse(const ImVec2& center,
                                  const ImVec2& radius,
                                  const Color& color,
                                  const float thickness = 1.0f)
{
  if (radius.x == radius.y) {
    draw_shadowed_circle(center, radius.x, color, thickness);
  }
  else {
    draw_ellipse(center + ImVec2 {0, thickness}, radius, kColorBlack, thickness);
    draw_ellipse(center, radius, color, thickness);
  }
}

inline void draw_shadowed_ellipse(const Float2& center,
                                  const Float2& radius,
                                  const Color& color,
                                  const float thickness = 1.0f)
{
  draw_shadowed_ellipse(as_imvec2(center), as_imvec2(radius), color, thickness);
}

/// Renders a region of a texture.
///
/// \param texture the source texture.
/// \param position the position of the rendered texture.
/// \param size the size of the rendered texture.
/// \param uv_min the normalized top-left corner of the texture region to render.
/// \param uv_max the normalized bottom-right corner of the texture region to render.
/// \param opacity the opacity of the rendered texture, in the interval [0, 255].
inline void render_image(const Texture& texture,
                         const ImVec2& position,
                         const ImVec2& size,
                         const ImVec2& uv_min = {0, 0},
                         const ImVec2& uv_max = {1, 1},
                         const uint8 opacity = 255)
{
  auto* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddImage(to_imgui_texture_id(texture.get_id()),
                      position,
                      position + size,
                      uv_min,
                      uv_max,
                      IM_COL32(0xFF, 0xFF, 0xFF, opacity));
}

inline void render_image(const Texture& texture,
                         const Float2& position,
                         const Float2& size,
                         const Float2& uv_min = {0, 0},
                         const Float2& uv_max = {1, 1},
                         const uint8 opacity = 255)
{
  render_image(texture,
               as_imvec2(position),
               as_imvec2(size),
               as_imvec2(uv_min),
               as_imvec2(uv_max),
               opacity);
}

}  // namespace tactile
