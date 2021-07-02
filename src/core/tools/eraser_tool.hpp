#pragma once

#include "mouse_tool.hpp"

namespace Tactile {

class EraserTool final : public AMouseTool
{
 public:
  explicit EraserTool(NotNull<Model*> model);

  [[nodiscard]] auto GetType() const -> MouseToolType override
  {
    return MouseToolType::Eraser;
  }
};

}  // namespace Tactile
