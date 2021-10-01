#pragma once

#include "common/tile_cache.hpp"
#include "common/tile_id.hpp"
#include "core/ctx/mouse_info.hpp"
#include "core/map_position.hpp"
#include "core/tool_type.hpp"

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
  MapPosition origin;
  TileID replacement;
};

/// \} End of group events

}  // namespace Tactile