#pragma once

#include <rune/everything.hpp>  // vector_map

#include "aliases/tile_id.hpp"
#include "core/map_position.hpp"

namespace Tactile {

struct StampSequenceEvent final
{
  rune::vector_map<MapPosition, tile_id> old_state;
  rune::vector_map<MapPosition, tile_id> sequence;
};

}  // namespace Tactile
