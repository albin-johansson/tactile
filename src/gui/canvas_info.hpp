#pragma once

#include "imgui.h"

namespace Tactile {

struct CanvasInfo final
{
  ImVec2 canvas_size{};
  ImVec2 canvas_tl{};  /// Top-left
  ImVec2 canvas_br{};  /// Bottom-right
};

[[nodiscard]] auto GetCanvasInfo() -> CanvasInfo;

}  // namespace Tactile
