#pragma once

#include "core/mouse_info.hpp"
#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"
#include "core/tool_type.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct MousePressedEvent final
{
  mouse_info info;
};

struct MouseDragEvent final
{
  mouse_info info;
};

struct MouseReleasedEvent final
{
  mouse_info info;
};

struct SelectToolEvent final
{
  tool_type type;
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
  tile_id replacement;
};

/// \} End of group events

}  // namespace tactile