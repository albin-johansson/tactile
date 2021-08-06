#pragma once

#include "core/tools/mouse_tool_type.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct SelectToolEvent final
{
  MouseToolType type;
};

/// \} End of group events

}  // namespace Tactile
