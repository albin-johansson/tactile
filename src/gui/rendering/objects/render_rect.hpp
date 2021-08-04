#pragma once

#include <imgui.h>

#include <centurion.hpp>  // frect

namespace Tactile {

class Object;

void RenderRect(const Object& object,
                const ImVec2& position,
                const cen::frect& bounds,
                ImU32 color,
                const ImVec2& ratio);

}  // namespace Tactile
