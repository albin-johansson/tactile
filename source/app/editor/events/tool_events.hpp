#pragma once

#include "core/mouse_info.hpp"
#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"
#include "core/tool_type.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct mouse_pressed_event final
{
  mouse_info info;
};

struct mouse_drag_event final
{
  mouse_info info;
};

struct mouse_released_event final
{
  mouse_info info;
};

struct select_tool_event final
{
  tool_type type{};
};

struct stamp_sequence_event final
{
  TileCache old_state;
  TileCache sequence;
};

struct eraser_sequence_event final
{
  TileCache old_state;
};

struct flood_event final
{
  tile_position origin;
  tile_id replacement{};
};

/// \} End of group events

}  // namespace tactile