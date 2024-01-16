// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <numbers>  // pi_v

#include <imgui.h>

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::ui {

/**
 * Renders a single N-gon.
 *
 * \param draw_list     The draw list to record the N-gon with.
 * \param center        The position of the N-gon center.
 * \param radius        The radius of the N-gon.
 * \param color         The line color to use.
 * \param segment_count The number of sides.
 * \param thickness     The line thickness.
 * \param angle         The rotation of the N-gon, in radians.
 */
TACTILE_CORE_API void draw_ngon(ImDrawList* draw_list,
                                const ImVec2& center,
                                float radius,
                                ImU32 color,
                                int segment_count,
                                float thickness,
                                float angle);

/**
 * Renders a single hexagon.
 *
 * \param draw_list The draw list to record the hexagon with.
 * \param center    The position of the hexagon center.
 * \param radius    The radius of the hexagon.
 * \param color     The line color to use.
 * \param thickness The line thickness.
 */
inline void draw_hexagon(ImDrawList* draw_list,
                         const ImVec2& center,
                         const float radius,
                         const ImU32 color,
                         const float thickness)
{
  draw_ngon(draw_list,
            center,
            radius,
            color,
            6,
            thickness,
            std::numbers::pi_v<float> * 0.5f);
}

}  // namespace tactile::ui
