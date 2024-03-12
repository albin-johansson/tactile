// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "primitives.hpp"

#include <cmath>    // sin, cos
#include <numbers>  // pi_v

namespace tactile {
namespace {

inline constexpr int kEllipseSegments = 50;
inline constexpr float kTau = std::numbers::pi_v<float> * 2.0f;

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

  const auto n_segments_f = static_cast<float>(n_segments);

  // Because we are filling a closed shape we remove 1 from the count of segments/points
  const float arc_max = kTau * (n_segments_f - 1.0f) / n_segments_f;
  path_elliptical_arc_to(self, center, radius, 0.0f, arc_max, n_segments - 1);

  self->PathStroke(color, true, thickness);
}

}  // namespace

void draw_ellipse(const ImVec2& center,
                  const ImVec2& radius,
                  const Color& color,
                  const float thickness)
{
  add_ellipse(ImGui::GetWindowDrawList(),
              center,
              radius,
              to_u32(color),
              kEllipseSegments,
              thickness);
}

}  // namespace tactile
