#pragma once

#include "aliases/tile_id.hpp"
#include "core/map_position.hpp"

namespace Tactile {

struct FloodEvent final
{
  MapPosition origin;
  tile_id replacement;
};

}  // namespace Tactile
