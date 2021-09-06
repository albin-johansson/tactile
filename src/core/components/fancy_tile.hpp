#pragma once

#include "aliases/tile_id.hpp"

namespace Tactile {

struct FancyTile final
{
  TileID id;  ///< The associated tile that has additional meta-information.
};

}  // namespace Tactile
