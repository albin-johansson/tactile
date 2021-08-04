#pragma once

#include <imgui.h>

namespace Tactile {

class Object;

void RenderEllipse(const Object& object,
                   const ImVec2& position,
                   const ImVec2& ratio,
                   ImU32 color);

}  // namespace Tactile
