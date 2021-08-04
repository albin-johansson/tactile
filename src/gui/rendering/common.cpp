#include "common.hpp"

#include <imgui_internal.h>

namespace Tactile {

void RenderShadowedCircle(const ImVec2& position,
                          const ImU32 color,
                          const float radius,
                          const float thickness)
{
  auto* drawList = ImGui::GetWindowDrawList();
  drawList->AddCircle(position + ImVec2{thickness, thickness},
                      radius,
                      IM_COL32_BLACK,
                      0,
                      thickness);
  drawList->AddCircle(position, radius, color, 0, thickness);
}

void RenderShadowedEllipse(const ImVec2& position,
                           const ImU32 color,
                           const float xRadius,
                           const float yRadius,
                           const float thickness,
                           const int nSegments)
{
  auto* drawList = ImGui::GetWindowDrawList();
  drawList->AddEllipse(position + ImVec2{thickness, thickness},
                       xRadius,
                       yRadius,
                       IM_COL32_BLACK,
                       nSegments,
                       thickness);
  drawList->AddEllipse(position, xRadius, yRadius, color, nSegments, thickness);
}

void RenderShadowedRect(const ImVec2& position,
                        const ImVec2& size,
                        const ImU32 color,
                        const float thickness)
{
  const auto offsetPosition = position + ImVec2{thickness, thickness};

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->AddRect(offsetPosition,
                    offsetPosition + size,
                    IM_COL32_BLACK,
                    0,
                    0,
                    thickness);
  drawList->AddRect(position, position + size, color, 0, 0, thickness);
}

}  // namespace Tactile
