// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/rendering.hpp"

namespace tactile::ui {
namespace {

inline constexpr float kTau = std::numbers::pi_v<float> * 2.0f;

}  // namespace

void draw_ngon(ImDrawList* draw_list,
               const ImVec2& center,
               const float radius,
               const ImU32 color,
               const int segment_count,
               const float thickness,
               const float angle)
{
  if ((color & IM_COL32_A_MASK) == 0 || segment_count <= 2) {
    return;
  }

  // Because we are filling a closed shape we remove 1 from the count of segments/points.
  const auto segment_max =
      static_cast<float>(segment_count - 1) / static_cast<float>(segment_count);
  const auto angle_max = kTau * segment_max;

  draw_list->PathArcTo(center, radius, angle, angle + angle_max, segment_count - 1);
  draw_list->PathStroke(color, ImDrawFlags_Closed, thickness);
}

}  // namespace tactile::ui
