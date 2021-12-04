#pragma once

#include "core/tool_type.hpp"

namespace Tactile {

struct ActiveTool final {
  ToolType tool{ToolType::None};
};

}  // namespace Tactile
