#pragma once

#include <rune/everything.hpp>  // vector_map

#include "aliases/tile_id.hpp"
#include "core/map_position.hpp"
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

struct StampSequenceEvent final
{
  rune::vector_map<MapPosition, tile_id> old_state;
  rune::vector_map<MapPosition, tile_id> sequence;
};

struct EraserSequenceEvent final
{
  rune::vector_map<MapPosition, tile_id> old_state;
};

/// \} End of group events

}  // namespace Tactile