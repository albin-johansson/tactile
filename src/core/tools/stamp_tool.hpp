#pragma once

#include "mouse_tool.hpp"

namespace Tactile {

class StampTool final : public AMouseTool
{
 public:
  [[nodiscard]] auto GetType() const -> MouseToolType override
  {
    return MouseToolType::Stamp;
  }
};

} //
