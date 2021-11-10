#include "common_rendering.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Tactile {

void PathEllipticalArcTo(ImDrawList* self,
                         const ImVec2& center,
                         const float radius_x,
                         const float radius_y,
                         const float a_min,
                         const float a_max,
                         const int num_segments = 10)
{
  // self->PathClear();
  // _Path.reserve(_Path.Size + (num_segments + 1));

  for (int i = 0; i <= num_segments; i++) {
    const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
    self->PathLineTo(
        ImVec2(center.x + ImCos(a) * radius_x, center.y + ImSin(a) * radius_y));
  }
}

void AddEllipse(ImDrawList* self,
                const ImVec2& center,
                const float radius_x,
                const float radius_y,
                const ImU32 col,
                const int num_segments = 12,
                const float thickness = 1.0f)
{
  if ((col & IM_COL32_A_MASK) == 0 || num_segments <= 2) {
    return;
  }

  // Because we are filling a closed shape we remove 1 from the count of segments/points
  const float a_max = IM_PI * 2.0f * (static_cast<float>(num_segments) - 1.0f) /
                      static_cast<float>(num_segments);
  PathEllipticalArcTo(self, center, radius_x, radius_y, 0.0f, a_max, num_segments - 1);
  self->PathStroke(col, true, thickness);
}

void AddEllipseFilled(ImDrawList* self,
                      const ImVec2& center,
                      const float radius_x,
                      const float radius_y,
                      const ImU32 col,
                      const int num_segments = 12)
{
  if ((col & IM_COL32_A_MASK) == 0 || num_segments <= 2) {
    return;
  }

  // Because we are filling a closed shape we remove 1 from the count of segments/points
  const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
  PathEllipticalArcTo(self, center, radius_x, radius_y, 0.0f, a_max, num_segments - 1);
  self->PathFillConvex(col);
}

void RenderShadowedCircle(const ImVec2& center,
                          const float radius,
                          const uint32 color,
                          const float thickness)
{
  auto* drawList = ImGui::GetWindowDrawList();
  drawList->AddCircle(center + ImVec2{thickness, thickness},
                      radius,
                      IM_COL32(0, 0, 0, color >> IM_COL32_A_SHIFT),
                      0,
                      thickness);
  drawList->AddCircle(center, radius, color, 0, thickness);
}

void RenderShadowedEllipse(const ImVec2& center,
                           const float xRadius,
                           const float yRadius,
                           const uint32 color,
                           const float thickness,
                           const int nSegments)
{
  if (xRadius == yRadius) {
    RenderShadowedCircle(center, xRadius, color, thickness);
  }
  else {
    auto* drawList = ImGui::GetWindowDrawList();
    AddEllipse(drawList,
               center + ImVec2{thickness, thickness},
               xRadius,
               yRadius,
               IM_COL32(0, 0, 0, color >> IM_COL32_A_SHIFT),
               nSegments,
               thickness);
    AddEllipse(drawList, center, xRadius, yRadius, color, nSegments, thickness);
  }
}

void RenderShadowedRect(const ImVec2& position,
                        const ImVec2& size,
                        const uint32 color,
                        const float thickness)
{
  const auto offsetPosition = position + ImVec2{thickness, thickness};

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->AddRect(offsetPosition,
                    offsetPosition + size,
                    IM_COL32(0, 0, 0, color >> IM_COL32_A_SHIFT),
                    0,
                    0,
                    thickness);
  drawList->AddRect(position, position + size, color, 0, 0, thickness);
}

}  // namespace Tactile
