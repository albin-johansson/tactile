#pragma once

#include <tactile_def.hpp>

#include "core/tile_position.hpp"
#include "core/mouse_info.hpp"
#include "core/tile_cache.hpp"
#include "core/tool_type.hpp"

namespace tactile {

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
  ToolType type;
};

struct StampSequenceEvent final
{
  TileCache old_state;
  TileCache sequence;
};

struct EraserSequenceEvent final
{
  TileCache old_state;
};

struct FloodEvent final
{
  tile_position origin;
  TileID replacement;
};

/// \} End of group events

}  // namespace tactile