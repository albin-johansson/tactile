#pragma once

#include "core/tool_type.hpp"

namespace tactile {

struct ActiveTool final
{
  tool_type tool{tool_type::none};
};

}  // namespace tactile
