#pragma once

#include "core/mouse_info.hpp"
#include "core/mouse_tool_type.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct MousePressedEvent final
{
  MouseInfo info;
};

struct MouseDragEvent final
{
  MouseInfo info;
};

struct MouseReleasedEvent final
{
  MouseInfo info;
};

struct SelectToolEvent final
{
  MouseToolType type;
};

/// \} End of group events

}  // namespace Tactile