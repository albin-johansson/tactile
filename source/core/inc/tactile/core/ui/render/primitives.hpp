// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <numbers>  // pi_v

#include <imgui.h>

#include "tactile/base/meta/color.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/meta/color.hpp"
#include "tactile/core/ui/imgui_compat.hpp"

namespace tactile::ui {

/**
 * Renders a rectangle.
 *
 * \param screen_pos The rectangle position, in screen-space.
 * \param size       The rectangle size.
 * \param color      The line color.
 * \param thickness  The line thickness.
 */
inline void draw_rect(const Float2& screen_pos,
                      const Float2& size,
                      const UColor& color,
                      const float thickness = 1.0f)
{
  if (auto* draw_list = ImGui::GetWindowDrawList()) {
    draw_list->AddRect(to_imvec2(screen_pos),
                       to_imvec2(screen_pos + size),
                       to_uint32_abgr(color),
                       0.0f,
                       ImDrawFlags_None,
                       thickness);
  }
}

/**
 * Renders a filled rectangle.
 *
 * \param screen_pos The rectangle position, in screen-space.
 * \param size       The rectangle size.
 * \param color      The fill color.
 */
inline void fill_rect(const Float2& screen_pos, const Float2& size, const UColor& color)
{
  if (auto* draw_list = ImGui::GetWindowDrawList()) {
    draw_list->AddRectFilled(to_imvec2(screen_pos),
                             to_imvec2(screen_pos + size),
                             to_uint32_abgr(color));
  }
}

/**
 * Renders an N-sided polygon.
 *
 * \param screen_pos    The center position of the polygon, in screen-space.
 * \param radius        The radius of the polygon.
 * \param color_mask    The ABGR line color mask.
 * \param segment_count The number of sides (the "N" in N-gon).
 * \param thickness     The line thickness.
 * \param angle         The polygon rotation, in radians.
 */
inline void draw_ngon(const Float2& screen_pos,
                      const float radius,
                      const uint32 color_mask,
                      const int segment_count,
                      const float thickness = 1.0f,
                      const float angle = 0.0f)
{
  if ((color_mask & IM_COL32_A_MASK) == 0 || segment_count <= 2) {
    return;
  }

  // We're drawing a closed shape, so we remove one of the segments/points.
  const auto segment_max =
      static_cast<float>(segment_count - 1) / static_cast<float>(segment_count);
  const auto angle_max = 2.0f * std::numbers::pi_v<float> * segment_max;

  if (auto* draw_list = ImGui::GetWindowDrawList()) {
    draw_list->PathArcTo(to_imvec2(screen_pos),
                         radius,
                         angle,
                         angle + angle_max,
                         segment_count - 1);
    draw_list->PathStroke(color_mask, ImDrawFlags_Closed, thickness);
  }
}

/**
 * Renders a hexagon.
 *
 * \details
 * The rendered hexagon will feature a vertex immediately above the provided center point, as
 * if rendered via \c draw_ngon with an angle of half Pi.
 *
 * \param screen_pos The center position of the hexagon, in screen-space.
 * \param radius     The radius of the hexagon.
 * \param color_mask The ABGR line color mask.
 * \param thickness  The line thickness.
 */
inline void draw_hexagon(const Float2& screen_pos,
                         const float radius,
                         const uint32 color_mask,
                         const float thickness = 1.0f)
{
  constexpr auto half_pi = 0.5f * std::numbers::pi_v<float>;
  draw_ngon(screen_pos, radius, color_mask, 6, thickness, half_pi);
}

}  // namespace tactile::ui
