#pragma once

#include <imgui.h>

namespace Tactile {

void RenderShadowedCircle(const ImVec2& position,
                          ImU32 color,
                          float radius,
                          float thickness);

void RenderShadowedEllipse(const ImVec2& position,
                           ImU32 color,
                           float xRadius,
                           float yRadius,
                           float thickness,
                           int nSegments);

void RenderShadowedRect(const ImVec2& position,
                        const ImVec2& size,
                        ImU32 color,
                        float thickness);

}  // namespace Tactile
