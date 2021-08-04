#pragma once

#include <imgui.h>

#include <centurion.hpp>  // frect

namespace Tactile {

class Object;

void RenderPoint(const Object& object,
                 const ImVec2& position,
                 const cen::frect& boundsRect,
                 ImU32 color,
                 float gridWidth);

}  // namespace Tactile
