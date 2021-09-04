#pragma once

#include "core/mouse_tool_type.hpp"

namespace Tactile {

struct ActiveTool final
{
  MouseToolType tool{MouseToolType::None};
};

}  // namespace Tactile
