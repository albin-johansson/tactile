#pragma once

#include "mouse_tool.hpp"

namespace Tactile {

class EraserTool final : public AMouseTool
{
 public:
  [[nodiscard]] auto GetType() const -> MouseToolType override
  {
    return MouseToolType::Eraser;
  }
};

}  // namespace Tactile
