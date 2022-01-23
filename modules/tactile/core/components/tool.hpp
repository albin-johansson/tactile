#pragma once

#include "core/tool_type.hpp"

namespace tactile {

struct ActiveTool final
{
  ToolType tool{ToolType::None};
};

}  // namespace tactile
