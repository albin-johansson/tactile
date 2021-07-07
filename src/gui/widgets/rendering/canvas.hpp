#pragma once

#include <imgui.h>

namespace Tactile {

struct CanvasInfo final
{
  ImVec2 size{};
  ImVec2 tl{};  /// Top-left
  ImVec2 br{};  /// Bottom-right
};

void FillBackground(const CanvasInfo& canvas);

[[nodiscard]] auto GetCanvasInfo() -> CanvasInfo;

}  // namespace Tactile
